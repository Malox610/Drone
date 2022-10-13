//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte addressE[6] = "node1";
const byte addressR[6] = "node2";

struct PayloadStruct {
  unsigned long nodeID;
  unsigned long payloadID;
};
PayloadStruct payload;

char role = 'E';
void ChangeRole();

void setup()
{
  Serial.begin(115200);
   if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

   radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload));  // 2x int datatype occupy 8 bytes

  // Set the pipe addresses accordingly. This function additionally also
  // calls startListening() or stopListening() and sets the payload's nodeID
  
  
}

void ChangeRole()
{
   if(role =='E') // if envoyeur 
    {
      
       Serial.println("Maintenant R1");
    role ='R' ;
    radio.openReadingPipe(0, addressR);
    radio.startListening();
    Serial.println("Maintenant R2");
    }

    if(role =='R') // if receveur
    {      Serial.println("Maintenant E1");
    role ='E' ; // changement de receveur a envoyer
    radio.openWritingPipe(addressE);
    radio.stopListening();
    Serial.println("Maintenant E2");
    }
  }
  
void loop()
{
  Serial.println(role);
  // delay(1000);
  
  if(role =='R') // if receveur
    {
        Serial.println("ya til un message");
       if (radio.available())
       {
         char text[32] = {0};
     radio.read(&text, sizeof(text));
         Serial.println(text);
              Serial.println(" radio");
         
        
         ChangeRole();
          delay(1000);
       }else
       {
         Serial.println("Pas radio");
        
        }
    } 

      if(role =='E') // if receveur
      {//Send message to receiver
        
         const char text[] = "Envoi Mathis";
        bool report2= radio.write(&text, sizeof(text));
         Serial.println("tets envoie ");
        if(report2)
        {
           Serial.println("Envoyer");
         ChangeRole();
        }
          delay(1000);
      } 
  
}
