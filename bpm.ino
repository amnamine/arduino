#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int pulsePin = A0;                 // Fic  l violet du capteur de pouls connecté à la broche analogique A0
int blinkPin = 13;                // broche pour faire clignoter la led à chaque battement

// les variables volatiles
volatile int BPM;                   // int qui contient l'analogique brut en 0. mis à jour toutes les 2 ms
volatile int Signal;                // contient les données brutes entrantes
volatile int IBI = 600;             // int qui contient l'intervalle de temps entre les battements 
volatile boolean Pulse = false;     // "True" lorsque le rythme cardiaque en direct de l'utilisateur est détecté. "Faux" lorsqu'il ne s'agit pas d'un "rythme en direct".
volatile boolean QS = false;        // devient vrai quand Arduino trouve un battement

static boolean serialVisual = true;   // Défini sur "faux" par défaut. Réinitialisez sur "true" pour voir Arduino Serial Monitor ASCII Visual Pulse
volatile int rate[10];                      // tableau pour contenir les dix dernières valeurs IBI
volatile unsigned long sampleCounter = 0;          // utilisé pour déterminer la synchronisation des impulsions
volatile unsigned long lastBeatTime = 0;           // utilisé pour trouver IBI
volatile int P = 512;                      // utilisé pour trouver IBI
volatile int T = 512;                     // utilisé pour trouver le creux dans l'onde de pouls, ensemencé
volatile int thresh = 525;                // utilisé pour trouver le moment instantané du battement de coeur, ensemencé
volatile int amp = 100;                   // utilisé pour maintenir l'amplitude de la forme d'onde d'impulsion, ensemencéeutilisé pour maintenir l'amplitude de la forme d'onde d'impulsion, ensemencée
volatile boolean firstBeat = true;        // utilisé pour ensemencer le tableau de taux afin que nous démarrions avec un BPM raisonnable
volatile boolean secondBeat = false;      //utilisé pour ensemencer le tableau de taux afin que nous démarrions avec un BPM raisonnable

void setup()
{
  pinMode(blinkPin,OUTPUT);         // broche qui clignotera à votre rythme cardiaque
  Serial.begin(115200);             // vitesse moniteur série
  interruptSetup();                 // se configure pour lire le signal du capteur de pouls toutes les 2 ms
                                     
 lcd.begin(20, 4); // initialisation du LCD
// lcd.setCursor(5,3);
// lcd.print("hello");
// delay(1000);
//lcd.clear();
// 
}



void loop()
{
   serialOutput();  
   
  if (QS == true) // Un battement de coeur a été trouvé
    {     
      // BPM et IBI ont été déterminés
      // Quantified Self "QS" vrai quand arduino trouve un battement de coeur
      serialOutputWhenBeatHappens();      
      QS = false; // réinitialiser le drapeau Quantified Self pour la prochaine fois  // Un battement s'est produit, sortie en série.
    }
     
  delay(20); //  délai
}


void interruptSetup()
{     
  //Initialise Timer2 pour lancer une interruption toutes les 2 ms.
  TCCR2A = 0x02;     // DÉSACTIVER PWM SUR LES BROCHES NUMÉRIQUES 3 ET 11 ET PASSER EN MODE CTC
  TCCR2B = 0x06;     // NE PAS FORCER LA COMPARAISON, 256 PRESCALER
  OCR2A = 0X7C;      // RÉGLEZ LE HAUT DU COMPTE À 124 POUR UN FRÉQUENCE D'ÉCHANTILLONNAGE DE 500 Hz
  TIMSK2 = 0x02;     // ACTIVER L'INTERRUPTION SUR CORRESPONDANCE ENTRE TIMER2 ET OCR2A
  sei();             // ASSUREZ-VOUS QUE LES INTERRUPTIONS GLOBALES SONT ACTIVÉES  
} 

void serialOutput()
{   // Décidez comment sortir en série.
 if (serialVisual == true)
  {  
     arduinoSerialMonitorVisual('-', Signal);   // va à la fonction qui rend Serial Monitor Visualizer
  } 
 else
  {
      sendDataToSerial('S', Signal);     // va à la fonction sendDataToSerial
   }        
}

void serialOutputWhenBeatHappens()
{    
 if (serialVisual == true) // Code pour faire fonctionner le visualiseur de moniteur série
   {            
     Serial.println(" Heart-Beat Found ");  //ASCII
     Serial.println("BPM: ");
     Serial.println(BPM);
delay(100);
     
     lcd.setCursor(0,0);
     lcd.print("HeartBeatFound");
     lcd.setCursor(3,2);
     lcd.print("BPM: ");
     lcd.setCursor(5,3);
     lcd.print(BPM);
delay(100);
//     lcd.clear();
     
   }
 else
   {
     sendDataToSerial('B',BPM);   // envoie la fréquence cardiaque avec un préfixe 'B
     sendDataToSerial('Q',IBI);   // envoie le temps entre les battements avec un préfixe 'Q'
   }   
}

void arduinoSerialMonitorVisual(char symbol, int data )
{    
  const int sensorMin = 0;      // capteur minimum, découvert par l'expérience
  const int sensorMax = 1024;    // capteur maximum, découvert par l'expérience
  int sensorReading = data; // mappe la plage du capteur sur une plage de 12 options :
  int range = map(sensorReading, sensorMin, sensorMax, 0, 11);
  // faire quelque chose de différent en fonction de la valeur de la plage
}


void sendDataToSerial(char symbol, int data )
{
   Serial.print(symbol);
   Serial.println(data);                
}

ISR(TIMER2_COMPA_vect) //déclenché lorsque Timer2 compte jusqu'à 124
{  
  cli();                                      // désactiver les interruptions pendant que nous faisons cela
  Signal = analogRead(pulsePin);              // lire le capteur de pouls
  sampleCounter += 2;                         // suivre le temps en mS avec cette variable
  int N = sampleCounter - lastBeatTime;       // surveiller le temps écoulé depuis le dernier battement pour éviter le bruit
                                              //  trouver le pic et le creux de l'onde de pouls
  if(Signal < thresh && N > (IBI/5)*3) // éviter le bruit dichrotique en attendant les 3/5 du dernier IBI
    {      
      if (Signal < T) // T est le creux
      {                        
        T = Signal; // garder une trace du point le plus bas de l'onde de pouls
      }
    }

  if(Signal > thresh && Signal > P)
    {          // la condition de seuil permet d'éviter le bruit
      P = Signal;                             // P est le pic
    }                                        // garder une trace du point le plus élevé de l'onde de pouls

  
  // le signal augmente en valeur chaque fois qu'il y a une impulsione
  if (N > 250)
  {                                   // éviter les bruits à haute fréquence
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) )
      {        
        Pulse = true;                               // définir le drapeau Pulse lorsque nous pensons qu'il y a une impulsion
        digitalWrite(blinkPin,HIGH);                // allumer la broche 13 LED
        IBI = sampleCounter - lastBeatTime;         // mesurer le temps entre les battements en mS
        lastBeatTime = sampleCounter;               // garder une trace du temps pour la prochaine impulsion
  
        if(secondBeat)
        {                        // s'il s'agit du deuxième temps, si secondBeat == TRUE
          secondBeat = false;                  // effacer le drapeau secondBeat
          for(int i=0; i<=9; i++) // ensemencer le total cumulé pour obtenir un BPM réaliste au démarrage
          {             
            rate[i] = IBI;                      
          }
        }
  
        if(firstBeat) // si c'est la première fois qu'on trouve un beat, if firstBeat == TRUE
        {                         
          firstBeat = false;                   // effacer le drapeau firstBeat
          secondBeat = true;                   // définir le deuxième drapeau de battement
          sei();                               // réactiver les interruptions
          return;                              // La valeur IBI n'est pas fiable, alors supprime
        }   
      // conserver un total cumulé des 10 dernières valeurs IBI
      word runningTotal = 0;                  // effacer la variable runningTotal  

      for(int i=0; i<=8; i++)
        {                // décaler les données dans le tableau de taux
          rate[i] = rate[i+1];                  // et supprimez la valeur IBI la plus ancienne
          runningTotal += rate[i];              // additionner les 9 valeurs IBI les plus anciennes
        }

      rate[9] = IBI;                          // ajouter le dernier IBI au tableau des tarifs
      runningTotal += rate[9];                // ajouter le dernier IBI à runningTotal
      runningTotal /= 10;                     // faire la moyenne des 10 dernières valeurs IBI
      BPM = 60000/runningTotal;               // combien de battements peuvent tenir dans une minute 
      QS = true;                              // définir l'indicateur Quantified Self 
      
    }                       
  }

  if (Signal < thresh && Pulse == true)
    {   // quand les valeurs baissent, le rythme est terminé
      digitalWrite(blinkPin,LOW);            // éteindre la broche 13 LED
      Pulse = false;                         // réinitialiser le drapeau Pulse pour que nous puissions le refaire
      amp = P - T;                           // obtenir l'amplitude de l'onde de pouls
      thresh = amp/2 + T;                    // seuil à 50% de l'amplitude
      P = thresh;                            // réinitialisez-les pour la prochaine fois
      T = thresh;
    }

  if (N > 2500)
    {                           // si 2,5 secondes passent sans battement
      thresh = 512;                          // définir le seuil par défaut
      P = 512;                               // définir P par défaut
      T = 512;                               // définir T par défaut
      lastBeatTime = sampleCounter;          // mettre à jour le lastBeatTime      
      firstBeat = true;                      // réglez-les pour éviter le bruit
      secondBeat = false;                    // quand on récupère le rythme cardiaque
    }

  sei();                                   // activez les interruptions lorsque vous avez terminé
}// fin isr
