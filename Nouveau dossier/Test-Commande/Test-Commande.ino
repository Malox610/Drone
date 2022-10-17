//source : https://passionelectronique.fr/tutorial-nrf24l01/ 

#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

#define pinCE     7         // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN    8         // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define pinSERVO  9         // On associe la broche "SIGNAL" du SERVO à la sortie digitale D9 de l'arduino
#define pinPOT    A0        // On associe le point milieu du potentiomètre à l'entrée analogique A0 de l'arduino

#define tunnel1 "PIPE1"     // On définit un premier "nom de tunnel" (5 caractères), pour pouvoir envoyer des données à l'autre NRF24
#define tunnel2 "PIPE2"     // On définit un second "nom de tunnel" (5 caractères), pour pouvoir recevoir des données de l'autre NRF24

RF24 radio(pinCE, pinCSN);  // Instanciation du NRF24L01
Servo servomoteur;          // Instanciation d'un objet pour contrôler le servomoteur

const byte adresses[][6] = {tunnel1, tunnel2};    // Tableau des adresses de tunnel, au format "byte array"

int valeurPotLocal;            // Variable contenant la valeur du potentiomètre
int valeurAngleServoLocal;     // Variable contenant la valeur de l'angle du servomoteur
int valeurAngleServoDistant;   // Variable contenant la valeur de l'angle du servomoteur

void setup() {
  pinMode(pinPOT, INPUT);         // Déclaration de la pin "pinPOT" en entrée
  servomoteur.attach(pinSERVO);   // Liaison de la pin "pinSERVO" au servomoteur branché sur l'arduino
  
  radio.begin();                           // Initialisation du module NRF24
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);   // Ouverture du "tunnel2" en LECTURE
  radio.setPALevel(RF24_PA_MIN);           // Sélection d'un niveau "MINIMAL" pour les essais mettre RF24_PA_MAX pour les vrai test
  radio.setChannel(96); // fréquence 2496 Mhz
  
  servomoteur.write(90);    // Rotation du servomoteur pour le mettre en position médiane
  delay(2000);              // puis démarrage du programme
}

void loop() {
  // ******** ENVOI ********
  radio.stopListening();                                                  // On commence par arrêter le mode écoute, pour pouvoir émettre les données
  valeurPotLocal = analogRead(pinPOT);                                    // On lit la valeur du potentiomètre (résultat retourné entre 0 et 1023)
  valeurAngleServoDistant = map(valeurPotLocal, 0, 1023, 0, 180);         // On converti la valeur [0..1023] en [0..180] (pour correspondre à l'angle d'un servo)
  valeurAngleServoDistant = 2*(int)(valeurAngleServoDistant/2);           // Léger arrondi, pour limiter les "tremblements" du servomoteur
  radio.write(&valeurAngleServoDistant, sizeof(valeurAngleServoDistant)); // … et on envoi cette valeur à l'autre arduino, via le NRF24
  delay(5);                                                               // avec une petite pause, avant de passer à la suite

  // ******** RÉCEPTION ********
  radio.startListening();                                                 // On commence par arrêter le mode envoi, pour pouvoir réceptionner des données
  if(radio.available()) {                                                 // On regarde si une donnée a été reçue
    while (radio.available()) {                                           // Si une donné est en attente de lecture, on va la lire
      radio.read(&valeurAngleServoLocal, sizeof(valeurAngleServoLocal));  // Lecture des données reçues, une par une
      servomoteur.write(valeurAngleServoLocal);                           // … et ajustement de l'angle du servomoteur à chaque fois
    }
    delay(20);                                                             // avec une petite pause, avant de reboucler
  }
  delay(5);
}
