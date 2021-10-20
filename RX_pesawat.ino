
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define led 10

const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(7,8);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int ch1_val, ch2_val, ch3_val, ch4_val;


struct Signal { 
   byte ch1; 
   byte ch2;
   byte ch3;
   byte ch4;
 };

Signal data;

void ResetData()
{
  data.ch1= 127;
  data.ch2= 127;
  data.ch3= 127;
  data.ch4= 0;
}

void setup(){

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
    
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS); 
  radio.setChannel(108);
  radio.setCRCLength(RF24_CRC_8);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(0, pipe);
  radio.startListening();
  ResetData();
    
 servo1.attach(5);
 servo2.attach(4);
 servo3.attach(6);
 servo4.attach(3);

    
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}

unsigned long lastRecvTime = 0;


void recvData(){
    
 while ( radio.available() ) { 
radio.read(&data, sizeof(Signal));
 lastRecvTime = millis(); }
}

void loop(){
    
     //sensor Batre
 int Batre = analogRead(A3);
 float voltage = Batre * 6.00/1023.00 * 3;
    
    // voltage
if (voltage < 11.50) {
digitalWrite(led, HIGH); }
else {
digitalWrite(led, LOW); }
delay(5);
    
 recvData(); 
 unsigned long now = millis(); 
 if ( now - lastRecvTime > 1000 ) { ResetData(); } 

    
ch1_val = map(data.ch1, 0, 255, 1000, 2000);
    
ch2_val = map(data.ch2, 0, 255, 1000, 2000);
    
ch3_val = map(data.ch3, 0, 255, 1000, 2000);
    
ch4_val = map(data.ch4, 0, 255, 1000, 2000);
 
servo1.write(ch1_val);
servo2.write(ch2_val);
servo3.write(ch3_val);
servo4.write(ch4_val);
    
}
