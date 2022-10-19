//Include Libraries
#include <SPI.h>

#include <RF24.h>

#define pinCE     5         // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN    4         // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino

#define tunnel1 "PIPE1"     // On définit un premier "nom de tunnel" (5 caractères), pour pouvoir envoyer des données à l'autre NRF24
#define tunnel2 "PIPE2"     // On définit un second "nom de tunnel" (5 caractères), pour pouvoir recevoir des données de l'autre NRF24


RF24 radio(pinCE, pinCSN);  // Instanciation du NRF24L01

const int SW1_pin = 2; 
const int X1_pin = A3; 
const int Y1_pin = A2; 
const int SW2_pin = 3;
const int X2_pin = A1;
const int Y2_pin = A0; 

const byte adresses[][6] = {tunnel1, tunnel2};    // Tableau des adresses de tunnel, au format "byte array"


void setup()
{
    Serial.begin(115200);
  
  int p1=0;
  radio.begin();
  radio.begin();                           // Initialisation du module NRF24
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);   // Ouverture du "tunnel2" en LECTURE
  radio.setPALevel(RF24_PA_MIN);           // Sélection d'un niveau "MINIMAL" pour les essais mettre RF24_PA_MAX pour les vrai test
  radio.setChannel(96); // fréquence 2496 Mhz
  
}
void loop()
{
 
 int PosX1 = 0;
 int PosY1 = 0;
 int PosX2 = 0;
 int PosY2 = 0;
 
  if(digitalRead(SW2_pin)==0){
  analogWrite(6,200);
  }else {
  analogWrite(6,0);
  }
  PosX1 = analogRead(X1_pin);
  PosY1 = analogRead(Y1_pin);
  PosX2 = analogRead(X2_pin);
  PosY2 = analogRead(Y2_pin);

  int Pos[] = {PosX1,PosY1,PosX2,PosY2};
  
  Pos[0] = map(PosX1, 0,1023,0,180);
  Pos[1] = map(PosY1, 0,1023,0,180);
  Pos[2] = map(PosX2, 0,1023,0,180);
  Pos[3] = map(PosY2, 0,1023,0,180);

  
  Serial.println(PosX1);
  Serial.println(PosY1);
  Serial.println(PosX2);
  Serial.println(PosY2);
  radio.stopListening();     
  radio.write(Pos, sizeof(Pos));
  delay(100);

  //récéption
 int hauteur =0;
 radio.startListening();                                                 // On commence par arrêter le mode envoi, pour pouvoir réceptionner des données
  if(radio.available()) {                                                 // On regarde si une donnée a été reçue
    while (radio.available()) {                                           // Si une donné est en attente de lecture, on va la lire
      radio.read(&hauteur, sizeof(hauteur));  // Lecture des donnéwaz es reçues, une par une
     Serial.print("Recu : ");
     Serial.println(hauteur);
    }
    delay(20);                                                             // avec une petite pause, avant de reboucler
  }


  
}
