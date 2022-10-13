//télécommande / transmitter

#include "SPI.h"
#include "NRFLite.h"

const static uint8_t RADIO_ID = 1;
const static uint8_t DESTINATION_RADIO_ID = 0;
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;

enum RadioPacketType
{
    AcknowledgementData, // Produced by the primary receiver and provided to the transmitter via an acknowledgement data packet.
    Heartbeat,    // Sent by the primary transmitter.
    BeginGetData, // Sent by the primary transmitter to tell the receiver it should load itself with an acknowledgement data packet.
    EndGetData    // Sent by the primary transmitter to receive the acknowledgement data packet from the receiver.
};

struct RadioPacket
{
    RadioPacketType PacketType;
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
};

NRFLite _radio;
uint32_t _lastHeartbeatSendTime, _lastDataRequestTime;

void setup()
{
    Serial.begin(115200);

    if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
    }
}

void loop()
{
    // Send a heartbeat once every second.
    if (millis() - _lastHeartbeatSendTime > 999)
    {
        _lastHeartbeatSendTime = millis();
        sendHeartbeat();
    }

    // Request data from the primary receiver once every 4 seconds.
    if (millis() - _lastDataRequestTime > 3999)
    {
        _lastDataRequestTime = millis();
        requestData();        
    }
}

void sendHeartbeat()
{
    Serial.print("Sending heartbeat");
    RadioPacket radioData;
    radioData.PacketType = Heartbeat;
    radioData.FromRadioId = RADIO_ID;
    radioData.OnTimeMillis = 144;
    bool se = _radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData));
    Serial.println(se);
    if (se==1)
    {
        Serial.println("...Success");
    }
    else
    {
        Serial.println("...prout");
    }
}

void requestData()
{
    Serial.println("Requesting data");
    Serial.print("  Sending BeginGetData");

    RadioPacket radioData;
    radioData.PacketType = BeginGetData; // When the receiver sees this packet type, it will load an ACK data packet.
    
    if (_radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData)))//problème ici
    {
        Serial.println("...Success");
        Serial.print("  Sending EndGetData");

        radioData.PacketType = EndGetData; // When the receiver acknowledges this packet, we will get the ACK data.

        if (_radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData)))
        {
            Serial.println("...Success");

            while (_radio.hasAckData()) // Look to see if the receiver provided the ACK data.
            {
                RadioPacket ackData;
                _radio.readData(&ackData);

                if (ackData.PacketType == AcknowledgementData)
                {
                    String msg = "  Received ACK data from ";
                    msg += ackData.FromRadioId;
                    msg += ", ";
                    msg += ackData.OnTimeMillis;
                    msg += " ms";
                    Serial.println(msg);
                }
            }
        }
        else
        {
            Serial.println("...raté");
        }
    }
    else
    {
        Serial.println("...encore raté");
    }    
}