#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);   //Module IIC/I2C Interface  0x3f
#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

int btn_start = 3;
int lastBtnStart;
int currentBtnStart;

int Add_Btn = 6;

    

int lastBtnAdd;
int currentBtnAdd;

int Reduce_Btn = 7;
int lastBtnReduce;
int currentBtnReduce;

int Relay_water = 4;
int Relay_water_Start = LOW;

int Relay_Fan = 5;
int Relay_Fan_Start = HIGH;

int Start = LOW;

int SetHumidity = 70;
int Humidity = dht.readHumidity();
int Temperature = dht.readTemperature();

void setup(){
 dht.begin();
 Serial.begin(9600);
 pinMode(btn_start,INPUT_PULLUP);
 pinMode(Relay_water,OUTPUT);
 pinMode(Relay_Fan,OUTPUT);
 pinMode(Add_Btn,INPUT_PULLUP);
 pinMode(Reduce_Btn,INPUT_PULLUP);
 
 currentBtnStart = digitalRead(btn_start);
 currentBtnAdd = digitalRead(Add_Btn);
 currentBtnReduce = digitalRead(Reduce_Btn);


 digitalWrite(Relay_water, HIGH);
 digitalWrite(Relay_Fan, LOW);
  lcd.init();
  lcd.backlight();
 
}


void loop()
{
  start();
  Serial.println(Start);
  if(Start == HIGH){
    Serial.println("start");
    lcd.setCursor(0,0);
    lcd.setCursor(0,1);
    lcd.print("");
    delay(500);
    Serial.print("Humidity :" +String(dht.readHumidity())  +  " %");
  Serial.print("\t");
  Serial.println("Temperture: " + String(dht.readTemperature()) + " C");
  lcd.setCursor(0,0); // กำหนดให้ เคอร์เซอร์ อยู่ตัวอักษรตำแหน่งที่0 แถวที่ 1 เตรียมพิมพ์ข้อความ
  lcd.print("Humidity:" +String(dht.readHumidity())  +  " %"); //พิมพ์ข้อความ "LCD1602 I2c Test"
  lcd.setCursor(0,1); // กำหนดให้ เคอร์เซอร์ อยู่ตัวอักษรกำแหน่งที3 แถวที่ 2 เตรียมพิมพ์ข้อความ
  lcd.print("Temperture: " + String(dht.readTemperature()) + " C");
  Relay_water_Start = HIGH;
    digitalWrite(Relay_water,Relay_water_Start);

    
  if(SetHumidity >= Humidity && 25 >= Temperature){
    Relay_water_Start = HIGH;
    digitalWrite(Relay_water,Relay_water_Start);
    Relay_Fan_Start = HIGH;
    digitalWrite(Relay_Fan,Relay_Fan_Start);  
     Serial.println("water on");
     Serial.println("Fan on");
    delay(10000);
    digitalWrite(Relay_water,!Relay_water_Start);
     Serial.println("water off");
     digitalWrite(Relay_Fan,!Relay_Fan_Start); 
    Serial.println("Fan off");
   }
   else if(SetHumidity >= Humidity && 25 <= Temperature){
    Relay_water_Start = HIGH;
    digitalWrite(Relay_water,Relay_water_Start);
    delay(10000);
    Serial.println("water on");

    digitalWrite(Relay_water,!Relay_water_Start);
    delay(10000);
    Serial.println("water off");
  }
  else if(SetHumidity <= Humidity && 25 >= Temperature){
    Relay_Fan_Start = HIGH;
    digitalWrite(Relay_water,Relay_water_Start);
    delay(10000);
    Serial.println("fan on");

    digitalWrite(Relay_Fan,!Relay_Fan_Start);
    delay(10000);
    Serial.println("fan off");
  }
}
 else if(Start == LOW){
   Serial.println("off & set mode");
   lcd.setCursor(0,0);
   lcd.print("Off                        ");
   lcd.setCursor(0,1);
   lcd.print("Set mode        ");
   Set_btn();
  }
  
}
void mainooo(){
  Serial.print("Ht :" +String(dht.readHumidity())  +  " %");
  Serial.print("\t");
  Serial.println("Temperture: " + String(dht.readTemperature()) + " C");
  lcd.setCursor(0, 0); // กำหนดให้ เคอร์เซอร์ อยู่ตัวอักษรตำแหน่งที่0 แถวที่ 1 เตรียมพิมพ์ข้อความ
  lcd.print("Ht :" +String(dht.readHumidity())  +  " %"); //พิมพ์ข้อความ "LCD1602 I2c Test"
  lcd.setCursor(0 , 1); // กำหนดให้ เคอร์เซอร์ อยู่ตัวอักษรกำแหน่งที3 แถวที่ 2 เตรียมพิมพ์ข้อความ
  lcd.print("Temperture:" + String(dht.readTemperature()) + " C");
  Relay_water_Start = HIGH;
    digitalWrite(Relay_water,Relay_water_Start);
  if(SetHumidity >= Humidity){
    Relay_water_Start = HIGH;
    digitalWrite(Relay_water,Relay_water_Start);
    Serial.println("water on");
    delay(10000);
    digitalWrite(Relay_water,!Relay_water_Start);
    Serial.println("water oof");
   }
   if(25 >= Temperature){  
    Relay_Fan_Start = HIGH;
    digitalWrite(Relay_Fan,Relay_Fan_Start);  
    Serial.println("Fan on");
    delay(10000);
    digitalWrite(Relay_Fan,!Relay_Fan_Start); 
    Serial.println("Fan off");
}
}

int start(){
  lastBtnStart = currentBtnStart;
  currentBtnStart = digitalRead(btn_start);
  if(lastBtnStart == HIGH && currentBtnStart == LOW){
    Start = !Start;
    
   }

   return Start;
}

void Set_btn(){
  lastBtnAdd = currentBtnAdd;
  currentBtnAdd = digitalRead(Add_Btn);
  
  if(SetHumidity >= Humidity && SetHumidity < 100){
   if(lastBtnAdd == HIGH && currentBtnAdd == LOW){
    SetHumidity++;
    delay(200);

    
   } 
  }

 
   lastBtnReduce = currentBtnReduce;
  currentBtnReduce = digitalRead(Reduce_Btn);
  
  if(SetHumidity > Humidity){
   if(lastBtnReduce == HIGH && currentBtnReduce == LOW){
    
    SetHumidity--;
    delay(200);

   } 
  }
  Serial.println(SetHumidity);
  lcd.setCursor(0, 0); // กำหนดให้ เคอร์เซอร์ อยู่ตัวอักษรตำแหน่งที่0 แถวที่ 1 เตรียมพิมพ์ข้อความ
    lcd.print("Ht Set :" +String(SetHumidity)  +  " %                ");
    lcd.setCursor(0,1);
    lcd.print("                ");
}
