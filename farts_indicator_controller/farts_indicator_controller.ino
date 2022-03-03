#include <RFM69.h>
#include <SPI.h>

#define NETWORKID 024

#define NODEID 32

#define RECEIVER 69

#define FREQUENCY RF69_915MHZ
#define IS_RFM69HCW true

#define ENCRYPTKEY "andrewANDsai1234"

#define RFM69_CS 10
#define RFM69_IRQ 2
#define RFM69_RST 9

#define SLEEP_LIGHTPIN 7
#define ANDREW_LIGHTPIN 6
#define SAI_LIGHTPIN 5
#define DND_LIGHTPIN 4

RFM69 farts = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW);

void setup() 
{
  Serial.begin(9600);
  
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  pinMode(SLEEP_LIGHTPIN, OUTPUT);
  pinMode(ANDREW_LIGHTPIN, OUTPUT);
  pinMode(SAI_LIGHTPIN, OUTPUT);
  pinMode(DND_LIGHTPIN, OUTPUT);

  farts.initialize(FREQUENCY, NODEID, NETWORKID);
  farts.setHighPower();
  farts.setPowerDBm(20);
  farts.setLNA(6);
  farts.encrypt(ENCRYPTKEY);

  // Changing the bit rate to 9.6kbps
  banana.writeReg(0x03, 0x0D); // Writes the top part of the bit rate to r3
  banana.writeReg(0x04, 0x05); // Writes the bottom part of the bit rate to r4

  Serial.println("Farts reporting for duty!");
}

void loop() 
{

  if(farts.receiveDone())
  {
    char mode[2];
    mode[0] = (char)*farts.DATA;
    mode[1] = (char)*(farts.DATA + 1);

    Serial.println(mode);
    
    if(mode[0] == 's')
    {
      if(mode[1] == 'n')
      {
        digitalWrite(SLEEP_LIGHTPIN, HIGH);
      } else if(mode[1] == 'a')
      {
        digitalWrite(ANDREW_LIGHTPIN, HIGH);
      } else if(mode[1] == 's')
      {
        digitalWrite(SAI_LIGHTPIN, HIGH);
      } else if(mode[1] == 'd')
      {
        digitalWrite(DND_LIGHTPIN, HIGH);
      }
    } 
    else if(mode[0] == 'r')
    {
      if(mode[1] == 'n')
      {
        digitalWrite(SLEEP_LIGHTPIN, LOW);
      } else if(mode[1] == 'a')
      {
        digitalWrite(ANDREW_LIGHTPIN, LOW);
      } else if(mode[1] == 's')
      {
        digitalWrite(SAI_LIGHTPIN, LOW);
      } else if(mode[1] == 'd')
      {
        digitalWrite(DND_LIGHTPIN, LOW);
      }
    }
  }

  farts.receiveDone();

}
