/* INTERNAL ARDUINO CODE
 *  WRITER: PAYTON FACK
 *  DATE  : 10/20/2022
 *  DESC  : Code for the Arduino located inside the back end compartment of the 
 *          Atonomous firefighting vehicle 
 */

#include<SPI.h>
#include<Ethernet.h>

//ids
#define INT_ARD_ID  0XA0
#define INT_COOLING 0XA1
#define INT_E_BRAKE 0XA2
#define INT_LIGHTS  0XA3
#define INT_SIREN   0XA4
#define INT_TEMP    0XA5

//utils
#define GET  0XF0
#define POST 0XF1
#define MASK 0X81

//err codes
#define OK  0xD0
#define ERR 0XD1

//Sets the ip port and MAC address of this device
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(120,10,10,1);
EthernetServer server(23);
EthernetClient client;

void setup() {
  //set up ethernet connection
  Ethernet.begin(mac, ip);
  //begin listening for connections from the pi
  server.begin();
  Serial.begin(9600);
}

void loop() {
  client = server.available(); //waits for a connection
  if(client){
    while(client.connected()){
      if(client.available()){
        char mask = client.read();

        //check if the mask was sent. This means a new message is starting
        Serial.println(mask & 0xff, HEX);
        if((0xff & mask) != MASK){
          return "";
        }
        //if a new message was detected
        else{
          //start by finding message length
          char len = client.read();
          Serial.print("New message received with length: ");
          Serial.println(len & 0xff, HEX);

          char type = client.read();

          char ardID = client.read();

          if((ardID & 0xff) != INT_ARD_ID ){
            Serial.print("ID does not match... command discarded ");
            return;
          }

          //if message is a GET
          if((type & 0xff) == GET){
            char sensorID = client.read();
            Serial.print("Message is type GET for sensor ");
            Serial.println(sensorID & 0xff, HEX);

            //pass to the data retrieval function

          }
          //if message is a POST
          if((type & 0xff) == POST){
            char servoID = client.read();
            char value = client.read();
            Serial.print("Message is type POST for servo ");
            Serial.print(servoID & 0xff, HEX);
            Serial.print(" and value ");
            Serial.println(value & 0xff, HEX);

            //pass to the actuation function

            //send ack if everything went ok
            char ackMess[] = {MASK, 0X03, servoID, OK, '\0'};
            client.write(ackMess);
            //client.stop();
          }
        }
      }
    }
  }
}
