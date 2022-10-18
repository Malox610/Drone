#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

#define pinCE       9       // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN      8       // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define pinSERVO1  11     // On associe la broche "SIGNAL" du SERVO à la sortie digitale D9 de l'arduino
#define pinSERVO2  10         // On associe la broche "SIGNAL" du SERVO à la sortie digitale D9 de l'arduino

#define pinPOT    A0        // On associe le point milieu du potentiomètre à l'entrée analogique A0 de l'arduino

#define tunnel1 "PIPE2"     // On définit un premier "nom de tunnel" (5 caractères), pour pouvoir envoyer des données à l'autre NRF24
#define tunnel2 "PIPE1"     // On définit un second "nom de tunnel" (5 caractères), pour pouvoir recevoir des données de l'autre NRF24
/* ----- NOTEZ L'INVERSION de PIPE1 et PIPE2, entre celui-ci et l'autre montage ! (car chaque tunnel véhicule les infos que dans un seul sens) */

RF24 radio(pinCE, pinCSN);  // Instanciation du NRF24L01
Servo servomoteur1;          // Instanciation d'un objet pour contrôler le servomoteur11
Servo servomoteur2; 

const byte adresses[][6] = {tunnel1, tunnel2};    // Tableau des adresses de tunnel, au format "byte array"


int valeurAngleServoLocal;     // Variable contenant la valeur de l'angle du servomoteur1
int valeurAngleServoDistant;   // Variable contenant la valeur de l'angle du servomoteur1

int hauteur =20;



void setup() {
  Serial.begin(115200);
  pinMode(pinPOT, INPUT);         // Déclaration de la pin "pinPOT" en entrée
  servomoteur1.attach(pinSERVO1);   // Liaison de la pin "pinSERVO" au servomoteur1 branché sur l'arduino
  servomoteur2.attach(pinSERVO2);   // Liaison de la pin "pinSERVO" au servomoteur1 branché sur l'arduino
  
  radio.begin();                           // Initialisation du module NRF24
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);   // Ouverture du "tunnel2" en LECTURE
  radio.setPALevel(RF24_PA_MIN);           // Sélection d'un niveau "MINIMAL" pour les essais mettre RF24_PA_MAX pour les vrai test
  radio.setChannel(96); // fréquence 2496 Mhz
  
  delay(1000);              // puis démarrage du programme
}

void loop() {
 int comm=14;
  // ******** ENVOI ********
  radio.stopListening();                                                  // On commence par arrêter le mode écoute, pour pouvoir émettre les données
  radio.write(&hauteur, sizeof(hauteur)); // … et on envoi cette valeur à l'autre arduino, via le NRF24
  delay(5);                                                               // avec une petite pause, avant de passer à la suite

  // ******** RÉCEPTION ********
  radio.startListening();                                                 // On commence par arrêter le mode envoi, pour pouvoir réceptionner des données
  if(radio.available()) {     //moyen que il n'y est rien                                            // On regarde si une donnée a été reçue
    while (radio.available()) {                                           // Si une donné est en attente de lecture, on va la lire
      radio.read(&comm, sizeof(comm));  // Lecture des données reçues, une par une
     Serial.println(comm);
    }
    delay(20);                                                             // avec une petite pause, avant de reboucler
  }
  delay(5);
}
