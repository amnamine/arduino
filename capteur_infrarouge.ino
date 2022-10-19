int ci = 3;
float lect;

bool  sortie=4;
void setup() {
pinMode(ci,INPUT);
pinMode(sortie,INPUT);
Serial.begin(9600);  
}

void loop() 
{
 lect= digitalRead(ci);
 Serial.println("premier=");
 Serial.println(lect);
 delay(500);
// Serial.println("deuxième");
// Serial.println(sortie);
//delay(500);
}
