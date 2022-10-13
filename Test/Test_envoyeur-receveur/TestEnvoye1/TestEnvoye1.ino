//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte addressE[6] = "node1";
const byte addressR[6] = "node2";

char role = 'E';
void ChangeRole()
{
   if(role =='E') // if envoyeur 
    {
    role ='R' ;
    radio.openReadingPipe(0, addressR);
    radio.startListening();
    }

    if(role =='R') // if receveur
    {
    role ='E' ; // changement de receveur a envoyer
    radio.openWritingPipe(addressE);
    radio.stopListening();
    }
  }
void setup()
{
  Serial.begin(115200);
  radio.begin();
  
  
}
void loop()
{
  if(role =='R') // if receveur
    {
       if (radio.available())
       {
         char text[32] = {0};
         radio.read(&text, sizeof(text));
         Serial.println(text);
         ChangeRole();
          delay(1000);
       }
    } 

      if(role =='E') // if receveur
      {//Send message to receiver
         const char text[] = "Hello World";
         radio.write(&text, sizeof(text));
         ChangeRole();
          delay(1000);
      } 
  
}
