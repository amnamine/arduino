
int bouton =A0;
int etat;


void setup() {
  pinMode(bouton,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  etat=digitalRead(bouton);
  Serial.println(etat);
  

}
