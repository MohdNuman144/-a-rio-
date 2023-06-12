#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

 
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
 
// Insert your network credentials
#define WIFI_SSID "qwertyui"
#define WIFI_PASSWORD "sdfghjk"
 
// Insert Firebase project API Key
#define API_KEY "AIzaSyB5p0U09BQbsmnkOtygugA2lbeFi62O2QsrVK8"
 
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://app-default-rtdb.firebaseio.com/" 

 
//Define Firebase Data object
FirebaseData fbdo;
 
FirebaseAuth auth;
FirebaseConfig config;
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int fsensor=D0;
int mq6sensor=A0;
int sensorvalue;
String Text="";
void setup(){
  Serial.begin(115200);
  pinMode(fsensor, INPUT);//define F_Sensor input pin
  pinMode(mq6sensor, INPUT);//MQ6 sensor
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  /* Assign the api key (required) */
  config.api_key = API_KEY;
 
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
 
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
 
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
 
void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
  int fire = digitalRead(fsensor);// read Flame sensor
 

   sensorvalue=analogRead(mq6sensor); /// read the MQ6 sensor
   Serial.println(sensorvalue);
  if(sensorvalue>50){
    Text="Gas Detected";
Firebase.RTDB.setString(&fbdo,"Gas Detector/Text",Text);
  Serial.println("Gas Detected");  
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.print("Gas Detected"); // Start Printing
  }
  else{
Text="Gas not Detected";
Firebase.RTDB.setString(&fbdo,"Gas Detector/Text",Text);
  Serial.println("Gas not Detected");
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.print("GasNotDetected"); // Start Printing
  }
    Firebase.RTDB.setFloat(&fbdo, "MQ6 Sensor/Value",sensorvalue);
}
}