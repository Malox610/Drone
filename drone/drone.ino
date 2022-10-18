//Include Libraries
#include <SPI.h>

#include <RF24.h>
#include <Servo.h> 


#define pinCE       9       // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN      8       // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino

#define tunnel1 "PIPE2"     // On définit un premier "nom de tunnel" (5 caractères), pour pouvoir envoyer des données à l'autre NRF24
#define tunnel2 "PIPE1"     // On définit un second "nom de tunnel" (5 caractères), pour pouvoir recevoir des données de l'autre NRF24

const byte adresses[][6] = {tunnel1, tunnel2};    // Tableau des adresses de tunnel, au format "byte array"


//create an RF24 object
RF24 radio(pinCE, pinCSN);  // Instanciation du NRF24L01
Servo myServoX1;
Servo myServoY1;
Servo myServoX2;
Servo myServoY2;


void setup()
{
  while (!Serial);
    Serial.begin(115200);
    myServoX1.attach(6);
    myServoY1.attach(5);
    myServoX2.attach(4);
    myServoY2.attach(3);
    
  radio.begin();                           // Initialisation du module NRF24
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);   // Ouverture du "tunnel2" en LECTURE
  radio.setPALevel(RF24_PA_MIN);           // Sélection d'un niveau "MINIMAL" pour les essais mettre RF24_PA_MAX pour les vrai test
  radio.setChannel(96); // fréquence 2496 Mhz
   delay(1000); 
}

void loop()
{
int PosX1 = 90;
int PosY1 = 90;
int PosX2 = 90;
int PosY2 = 90;

  //Read the data if available in buffer
  radio.startListening();
  if (radio.available())
  {
    
  int Pos[] = {PosX1,PosY1,PosX2,PosY2};
  
    radio.read(Pos, sizeof(Pos));

    PosX1 = Pos[0];
    PosY1 = Pos[1];
    PosX2 = Pos[2];
    PosY2 = Pos[3];
    
    myServoX1.write(PosX1);
    myServoY1.write(PosY1);  
    myServoX2.write(PosX2);
    myServoY2.write(PosY2);

  Serial.println(PosX1);
  Serial.println(PosY1);
  Serial.println(PosX2);
  Serial.println(PosY2);  

  }
 int hauteur =4;
   // ******** ENVOI ********
  radio.stopListening();                                                  // On commence par arrêter le mode écoute, pour pouvoir émettre les données
  radio.write(&hauteur, sizeof(hauteur)); // … et on envoi cette valeur à l'autre arduino, via le NRF24
  delay(20); 
  
}
