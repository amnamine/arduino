#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,16,2);
#define RST   9
#define S_DATA    10
#define RST2   4
#define S_DATA2    5
#define rxPin 11 // Broche 11 en tant que RX, à raccorder sur TX du HC-05
#define txPin 10 // Broche 10 en tant que TX, à raccorder sur RX du HC-05

int  ismustapha = 0;
int isilhem = 0;
int ismalak = 0; 
 
String tagUID = "";
byte Card[4];
String tagUID2 = "";
byte Card2[4];

SoftwareSerial mySerial(rxPin, txPin);
MFRC522 rfid1(S_DATA, RST);
MFRC522 rfid2(S_DATA2, RST2);
Servo myServo;
Servo myServo2;
int red1=22; int green1=23; int red2=24; int green2=25; int red3=26; int green3=27; int red4=28; int green4=29;
void setup() {
 // pinMode(A0,INPUT);
 pinMode(22,OUTPUT);
 pinMode(23,OUTPUT);
 pinMode(24,OUTPUT);
 pinMode(25,OUTPUT);
 pinMode(26,OUTPUT);
 pinMode(27,OUTPUT);
 pinMode(28,OUTPUT);
 pinMode(29,OUTPUT);
 pinMode(rxPin, INPUT);
 pinMode(txPin, OUTPUT);
 pinMode(3,OUTPUT);
 
lcd.init();
lcd.backlight();
  SPI.begin();       
  rfid1.PCD_Init();
  rfid2.PCD_Init();
  myServo.attach(8);
  myServo2.attach(6); 
  myServo.write(105);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("Bienvenue au");
lcd.setCursor(1,1);
lcd.print("MADINA PARKING"); 

Serial.begin(9600);
mySerial.begin(9600);
}

void loop() { 
int i = 0;
char someChar[32] = {0};

if (Serial.available()) {
   do{
    someChar[i++] = Serial.read();

   delay(3);
   
   }while (Serial.available() >0);

   mySerial.println(someChar);
   Serial.println(someChar);

}

while(mySerial.available())
   Serial.print((char)mySerial.read());
   

 /*int val=analogRead(A0);
 val =map(val,0,10,0,105);
 delay(10);
 myServo.write(val);
 delay(100);*/
    if ( rfid1.PICC_IsNewCardPresent()  &&  rfid1.PICC_ReadCardSerial()) {
   
    tagUID = "";
    
    for ( uint8_t i = 0; i < 4; i++) {  
      Card[i] = rfid1.uid.uidByte[i];
      tagUID.concat(String(rfid1.uid.uidByte[i], HEX));
    }
    tagUID.toUpperCase();
    rfid1.PICC_HaltA(); 
Serial.println(tagUID);
if (tagUID == "EF98D5CE")
{
  if(ismustapha == 0){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("acces autorise");
  mySerial.println("Acces autorise");
    lcd.setCursor(0,1);
  lcd.print("Mustapha place 1"); 
  mySerial.println("Mustapha place 1");
    myServo.write(10);
    delay(1000);
     myServo.write(105);
    ismustapha = 1;
 digitalWrite(29,HIGH);delay(500);
 digitalWrite(22,HIGH);
 digitalWrite(24,HIGH); 
 digitalWrite(26,HIGH); 
  /*digitalWrite(28,HIGH);*/ delay(5000);
     digitalWrite(29,LOW);
 digitalWrite(23,LOW);
 digitalWrite(22,LOW); 
 digitalWrite(24,LOW);
 digitalWrite(26,LOW);  
  //digitalWrite(28,LOW); 
    }
    else{
       lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MUSTAPHA occupe");
  mySerial.println("MUSTAPHA occupe");
  delay(2000);
      }
    lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
  lcd.setCursor(1,1);
  lcd.print("MADINA PARKING"); 
  }
  else if(tagUID == "3195594")
{
  if(isilhem == 0){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("acces autorise");
  mySerial.println("acces autorise");
    lcd.setCursor(0,1);
  lcd.print("ILHEM place 02"); 
   mySerial.println("ILHEM place 02");
    myServo.write(10);
    delay(1000);
    myServo.write(105);
    isilhem = 1;
     digitalWrite(27,HIGH);delay(500);
 digitalWrite(22,HIGH);
 digitalWrite(24,HIGH); 
  digitalWrite(28,HIGH); delay(5000);
     digitalWrite(27,LOW);
 digitalWrite(22,LOW); 
 digitalWrite(24,LOW); 
  digitalWrite(28,LOW);
  }else{
    lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("ILHEM occupe");
   mySerial.println("ILHEM occupe");
  delay(2000);
    }
    lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
  lcd.setCursor(1,1);
  lcd.print("MADINA PARKING"); 
}
 else if(tagUID == "EE85DA4")
{
  if(ismalak < 2){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("acces autorise");
   mySerial.println("acces autorise");
    lcd.setCursor(0,1);
  lcd.print("MALAK place 03"); 
   mySerial.println("MALAK place 03");
    myServo.write(10);
    delay(1000);
    myServo.write(105);
    ismalak++;
  /*   digitalWrite(25,HIGH);delay(500);
 digitalWrite(22,HIGH);
 digitalWrite(26,HIGH); 
  digitalWrite(28,HIGH); delay(5000);
     digitalWrite(25,LOW);
 digitalWrite(22,LOW); 
 digitalWrite(26,LOW); 
  digitalWrite(28,LOW);*/
      lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("acces autorise");
    lcd.setCursor(0,1);
  lcd.print("MALAK place 04"); 
    myServo.write(10);
    delay(1000);
    myServo.write(105);
         digitalWrite(23,HIGH);delay(500);
 digitalWrite(24,HIGH);
 digitalWrite(26,HIGH); 
  digitalWrite(28,HIGH); delay(5000);
     digitalWrite(23,LOW);
 digitalWrite(24,LOW); 
 digitalWrite(26,LOW); 
  digitalWrite(28,LOW);
  }else{
    lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("MALAK occupe");
   mySerial.println("Malak occupe");
  delay(2000);
    }
    lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
  lcd.setCursor(1,1);
  lcd.print("MADINA PARKING"); 
  
} else
  {tone(3, 70000, 6000);
lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("acces refuse");
   mySerial.println("acces refuse");
  lcd.setCursor(2,1);
  lcd.print("fausse carte");
   mySerial.println("fausse carte");
  delay(1000);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
     lcd.setCursor(1,1);
  lcd.print("MADINA PARKING");

    }
}

////////////////////////////////////////////////////////////////////
 if ( rfid2.PICC_IsNewCardPresent()  &&  rfid2.PICC_ReadCardSerial()) {
   
    tagUID2 = "";
    
    for ( uint8_t i = 0; i < 4; i++) {  
      Card2[i] = rfid2.uid.uidByte[i];
      tagUID2.concat(String(rfid2.uid.uidByte[i], HEX));
    }
    tagUID2.toUpperCase();
    rfid2.PICC_HaltA(); 
    if (tagUID2 == "EF98D5CE")
{
  if( ismustapha == 1){
 ismustapha = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("sortie autorisee");
   mySerial.println("sortie autorise");
  lcd.setCursor(0,1);
  lcd.print("Mustapha sorti"); 
   mySerial.println("mustapha sorti");
    myServo2.write(10);
    delay(1000);
    myServo2.write(105);
  }
  else{
    lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("voiture sortie");
   mySerial.println("voiture sortie");
  delay(2000);
    }
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
  lcd.setCursor(1,1);
  lcd.print("MADINA PARKING"); 
  }
  else if(tagUID2 == "3195594")
{
  if(isilhem == 1 ){
  isilhem = 0;
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("sortie autorisee");
   mySerial.println("sortie autorisee");
    lcd.setCursor(0,1);
  lcd.print("ILHEM sortie"); 
   mySerial.println("ILHEM sortie");
    myServo2.write(10);
    delay(1000);
    myServo2.write(105);
  }
  else{
        lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("voiture sortie");
   mySerial.println("voiture sortie");
  delay(2000);
    }
    lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
  lcd.setCursor(1,1);
  lcd.print("MADINA PARKING"); 
}
 else if(tagUID2 == "EE85DA4")
{
  if(ismalak > 0){
  ismalak--;
  if(ismalak < 0) ismalak = 0;
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("sortie autorisee");
   mySerial.println("sortie autorise");
    lcd.setCursor(0,1);
  lcd.print("MALAK sortie");
   mySerial.println("Malak sortie"); 
    myServo2.write(10);
    delay(1000);
    myServo2.write(105);
  }
  else{
     lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("voiture sortie");
   mySerial.println("voiture sortiee");
  delay(2000);
    }
    lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
  lcd.setCursor(1,1);
  lcd.print("MADINA PARKING"); 
  
} else
  {tone(3, 70000, 6000);
lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("acces refuse");
   mySerial.println("acces refuse");
  lcd.setCursor(1,1);
  lcd.print("fausse carte");
 mySerial.println("acces refuse");
  delay(1000);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bienvnue au");
     lcd.setCursor(1,1);
  lcd.print("MADINA PARKING");

    }

}
}


 
