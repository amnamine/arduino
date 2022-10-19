int capteurdesound = A0 ;
int capteurdesound2= 0;
int lecture1;
int lecture2;
int led= 12;

void setup() 
{ 
  pinMode (capteurdesound,INPUT); 
    pinMode (capteurdesound2,INPUT); 
  Serial.begin(9600);
  pinMode (led ,OUTPUT);
  
 

}

void loop() { 
  
Serial.println("capteur ANALOgIQUE");
lecture1 = analogRead (capteurdesound);
Serial.println(lecture1);
delay(500);

Serial.println("capteur DIGITAL");
lecture2 = digitalRead (capteurdesound2);
  Serial.println(lecture2);
  delay(500);
//if (lecture == 1 )
//{
// digitalWrite (led,HIGH)  ;
//}  
// else { 
//  digitalWrite(led,LOW );
}
