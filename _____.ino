#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2 // กำหนดให้ เซนเซอร์DETอยู่ PIN ที่2
DHT dht(DHTPIN, DHTTYPE);

int btn_start = 3; // กำหนดให้ ปุ่มเปิดปิดอยู่ PIN ที่3
int lastBtnStart;
int currentBtnStart;
int Start = LOW; // กำหนดค่าเริ่มต้นการใช้การเป็น LOW (ปิด)

int Add_Btn = 6;// กำหนดให้ ปุ่มเพิ่มความชื้นอยู่ PIN ที่6
int lastBtnAdd;
int currentBtnAdd;

int Reduce_Btn = 7;// กำหนดให้ ปุ่มลดความชื้นอยู่ PIN ที่7
int lastBtnReduce;
int currentBtnReduce;

int Relay_water = 4;// กำหนดให้ ปุ่มเปิดปิดrelayของปั้มน้ำอยู่ PIN ที่4


int Relay_Fan = 5;// กำหนดให้ ปุ่มเปิดปิดrelayของพัดลมอยู่ PIN ที่5
  



int SetHumidity = 70; //กำหนดค่าเริ่มต้มความชื่้น เป็น 70%
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

  lcd.init();
  lcd.backlight();
 
}


void loop()
{
  
  start();//เรียกใช้งานฟังชั้น start
  if(Start == HIGH){ //ถ้าตัวแปร Start เป็น HIGH ให้ ...
    Serial.println("start");//แสดงคำว่า start ใน console
    lcd.setCursor(0,0); // เซิตเคอร์เซอร์ให้อยู่ตำแหล่งที่ 1 บรรทัดที่ 1
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.clear();
    delay(500);
    Serial.print("Humidity :" +String(dht.readHumidity())  +  " %");//แสดงค่าความชื้นที่วัดได้ ใน console
    Serial.print("\t");
    Serial.println("Temperture: " + String(dht.readTemperature()) + " C");//แสดงค่าอุณหภูมิที่วัดได้ ใน console
    lcd.setCursor(0,0); // เซิตเคอร์เซอร์ให้อยู่ตำแหล่งที่ 1 บรรทัดที่ 1
    lcd.print("Humidity:" +String(dht.readHumidity())  +  " %"); //แสดงค่าความชื้นที่วัดได้ ใน จอLCD
    lcd.setCursor(0,1);// เซิตเคอร์เซอร์ให้อยู่ตำแหล่งที่ 1 บรรทัดที่ 2
    lcd.print("Temperture: " + String(dht.readTemperature()) + " C");//แสดงค่าอุณหภูมิที่วัดได้ ใน จอLCD


    
    if(SetHumidity >= Humidity && 25 >= Temperature){ //ถ้าค่าความชื้นที่ตั้งเอาไว้มากกว่าหรือเท่ากับค่าความชิ้นที่่วัด และ 25 มากกว่าหรือเท่ากับค่าอุณหภูมิที่วัด ให้ ...
      digitalWrite(Relay_water,HIGH);//เปิดน้ำ
      digitalWrite(Relay_Fan,LOW); //เปิดพัดลม
      Serial.println("water on");//แสดงคำว่า water on ใน console
      Serial.println("Fan on");//แสดงคำว่า fan on ใน console
      delay(10000);//ดีเลย์ 10วินาที
      digitalWrite(Relay_water,LOW);//ปิดน้ำ
      Serial.println("water off");//แสดงคำว่า water off ใน console
      digitalWrite(Relay_Fan,HIGH); //ปิดพัดลม
      Serial.println("Fan off");//แสดงคำว่า fan off ใน console
    }
    else if(SetHumidity >= Humidity && 25 <= Temperature){ //ถ้าค่าความชื้นที่ตั้งเอาไว้มากกว่าหรือเท่ากับค่าความชิ้นที่่วัด และ 25 น้อยกว่าหรือเท่ากับค่าอุณหภูมิที่วัด ให้ ...
      digitalWrite(Relay_water,HIGH);//เปิดน้ำ
      Serial.println("water on");//แสดงคำว่า water on ใน console
      delay(10000);//ดีเลย์ 10วินาที
      digitalWrite(Relay_water,!HIGH);//ปิดน้ำ
      Serial.println("water off");//แสดงคำว่า water off ใน console
    }
    else if(SetHumidity <= Humidity && 25 >= Temperature){ //ถ้าค่าความชื้นที่ตั้งเอาไว้น้อยกว่าหรือเท่ากับค่าความชิ้นที่่วัด และ 25 มากกว่าหรือเท่ากับค่าอุณหภูมิที่วัด ให้ ...
      digitalWrite(Relay_Fan,LOW);//เปิดพัดลม
      Serial.println("fan on");//แสดงคำว่า fan on ใน console
      delay(10000);//ดีเลย์ 10วินาที
      digitalWrite(Relay_Fan,HIGH); //ปิดพัดลม
      Serial.println("fan off");//แสดงคำว่า fan off ใน console
    }
  }
  else if(Start == LOW){ //ถ้าตัวแปร Start มีค่าเป็น LOW ให้ ...
   Set_Mode();// เรียกใช้งานฟังชั้น Set_Mode
   Serial.println("off & set mode"); //แสดงคำว่า off & set mode ใน console
   lcd.setCursor(0,0);// เซิตเคอร์เซอร์ให้อยู่ตำแหล่งที่ 1 บรรทัดที่ 1
   lcd.print("Off                        ");//แสดงคำว่า off ใน จอLCD
   lcd.setCursor(0,1);//เซิตเคอร์เซอร์ให้อยู่ตำแหล่งที่ 1 บรรทัดที่ 2
   lcd.print("Set mode        ");//แสดงคำว่า set mode ใน จอLCD
   delay(2000);//ดีเลย์ 2 วินาที
   
   lcd.setCursor(0,0);// เซิตเคอร์เซอร์ให้อยู่ตำแหล่งที่ 1 บรรทัดที่ 1
   lcd.clear();
   lcd.setCursor(0,1);//เซิตเคอร์เซอร์ให้อยู่ตำแหล่งที่ 1 บรรทัดที่ 2
   lcd.clear();
   
  }
  
}

int start(){
  lastBtnStart = currentBtnStart;
  currentBtnStart = digitalRead(btn_start);
  if(lastBtnStart == HIGH && currentBtnStart == LOW){ //ถ้ากดปุ่ม start ให้ ...
    Start = !Start;//สลับค่าปัจจุบัน
    
   }

   return Start; //ส่งออกให้ข้อมูลในตัวแปร Start
}

int Set_Mode(){
  lastBtnAdd = currentBtnAdd;
  currentBtnAdd = digitalRead(Add_Btn);

  if(SetHumidity >= 30 && SetHumidity < 100){ 
   if(lastBtnAdd == HIGH && currentBtnAdd == LOW){
    SetHumidity++;
    delay(200);
   } 
  }

  
  if(SetHumidity > 30){
   if(lastBtnReduce == HIGH && currentBtnReduce == LOW){
    
    SetHumidity--;
    delay(200);

   } 
  }
  Serial.println(SetHumidity);
  lcd.setCursor(0, 0); 
  lcd.print("Ht Set :" +String(SetHumidity)  +  " %                ");
  lcd.setCursor(0,1);
  lcd.clear();
  delay(2000);
  lcd.setCursor(0, 0); 
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.clear();
  return SetHumidity;
}
