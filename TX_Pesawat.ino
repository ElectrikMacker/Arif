/*
----- by ELECTRIK MACKER -------

  layar oled I2c
  gunakan pin A4 & A5 utk koneksi ke layar.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire,OLED_RESET);
 

const uint64_t pipeOut = 0xE8E8F0F0E1LL;

RF24 radio(7, 8);

struct Signal{

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
  data.ch4= 127;
}

#define ch1_in 5
#define ch2_in 6
#define ch3_in 10
#define ch4_in 9

bool ch1_invert = false;
bool ch2_invert = false;
bool ch3_invert = false;
bool ch4_invert = false;

int invert_counter = 0;

void setup(){
    
    Wire.begin();
display.begin(SSD1306_SWITCHCAPVCC,0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.display();
    delay(100);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
 
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS); 
  radio.setChannel(108);
  radio.setCRCLength(RF24_CRC_8); 
  radio.openWritingPipe(pipeOut);  
  radio.setPALevel(RF24_PA_MAX); 
  radio.stopListening();
  ResetData();
  
   pinMode(ch1_in, INPUT_PULLUP);
   pinMode(ch2_in, INPUT_PULLUP);
   pinMode(ch3_in, INPUT_PULLUP);
   pinMode(ch4_in, INPUT_PULLUP);
    
 }
  
int map_normal(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 127);
  else
    val = map(val, middle, upper, 127, 255);
  return ( reverse ? 255 - val : val );
}
void loop(){
   
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,16);
    display.print("Thr: ");
    display.print(data.ch4);
    
    display.setCursor(0,29);
    display.print("Yaw: ");
    display.print(data.ch3);
    
    display.setCursor(80,16);
    display.print("Pit: ");
    display.print(data.ch1);
    
    display.setCursor(80,29);
    display.print("Rol: ");
    display.print(data.ch2);
    
    display.setCursor(20,0);
    display.print("ELEKTRIC MACKER");
    display.display();
    
//data.ch1 = map(analogRead(A0), 0, 1023, 255*2, ch1_invert);
data.ch1= map_normal(analogRead(A0), 0, 512, 1023, ch1_invert);
data.ch2= map_normal(analogRead(A1), 0, 512, 1023, ch2_invert);
data.ch3= map_normal(analogRead(A2), 0, 512, 1023, ch3_invert);
data.ch4= map_normal(analogRead(A3), 0, 512, 1023, ch4_invert);


 if(!digitalRead(ch1_in)){
  ch1_invert = !ch1_invert;
 invert_counter = 0;
 invert_counter = invert_counter +1;
   delay(300);
    }
    
 if(!digitalRead(ch2_in)){
  ch2_invert = !ch2_invert;
 invert_counter = 0;
 invert_counter = invert_counter +1;
   delay(300);
    }
    
 if(!digitalRead(ch3_in)){
  ch3_invert = !ch3_invert;
 invert_counter = 0;
 invert_counter = invert_counter +1;
   delay(300);
    }
    
 if(!digitalRead(ch4_in)){
  ch4_invert = !ch4_invert;
 invert_counter = 0;
 invert_counter = invert_counter +1;
   delay(300);
    }
 
radio.write(&data, sizeof(Signal));
}

