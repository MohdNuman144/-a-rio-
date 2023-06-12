#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


int smokeA0=A0;
int buzzer =11;
int sensorValue;

void setup() {

  pinMode(buzzer,OUTPUT);
  pinMode(smokeA0,INPUT);
  Serial.begin(9600);
  Serial.println("Gas sensor warming up!");
  delay(20000); // allow the MQ-6 to warm up
  noTone(buzzer);

}
void loop() {
  int sensorValue=analogRead(smokeA0);
  String dataString = String(sensorValue);
  
  if(sensorValue > 300)
  {
    Serial.println(dataString);
    Serial.print(" | Gas detected!");
    lcd.init();
	  lcd.backlight();
	  lcd.clear();
	  lcd.setCursor(1,0);
    lcd.print("Gas detected");
    tone(buzzer,1000,2000);
  }
  else
  {
     Serial.println(dataString);
     Serial.println(" | Gas  not detected!");
     lcd.init();
	   lcd.backlight();
	   lcd.clear();
	   lcd.setCursor(1,0);     
     lcd.print("Gasnotdetected");

  
    noTone(buzzer);
   
  }
  delay(1000); // wait 2s for next reading
}