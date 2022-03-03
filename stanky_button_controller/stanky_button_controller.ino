#include <RFM69.h>
#include <SPI.h>

#define NETWORKID 024

#define NODEID 69

#define RECEIVER 32

#define FREQUENCY RF69_915MHZ
#define IS_RFM69HCW true

#define ENCRYPTKEY "andrewANDsai1234"

#define RFM69_CS 10
#define RFM69_IRQ 2
#define RFM69_RST 9

#define SLEEP_BUTTONPIN 7
#define ANDREW_BUTTONPIN 6
#define SAI_BUTTONPIN 5
#define DND_BUTTONPIN 4

bool sleep_isLit = false;
bool andrew_isLit = false;
bool sai_isLit = false;
bool dnd_isLit = false;

RFM69 stanky = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW);

void setup() 
{
  Serial.begin(9600);
  
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  pinMode(SLEEP_BUTTONPIN, INPUT);
  pinMode(ANDREW_BUTTONPIN, INPUT);
  pinMode(SAI_BUTTONPIN, INPUT);
  pinMode(DND_BUTTONPIN, INPUT);

  stanky.initialize(FREQUENCY, NODEID, NETWORKID);
  stanky.setHighPower();
  stanky.setPowerDBm(20);
  stanky.setLNA(6);
  stanky.encrypt(ENCRYPTKEY);

  // Changing the bit rate to 9.6kbps
  banana.writeReg(0x03, 0x0D); // Writes the top part of the bit rate to r3
  banana.writeReg(0x04, 0x05); // Writes the bottom part of the bit rate to r4

  Serial.println("Stanky reporting for duty!");
}

void loop() 
{
  int sleep_buttonRead = digitalRead(SLEEP_BUTTONPIN);
  int andrew_buttonRead = digitalRead(ANDREW_BUTTONPIN);
  int sai_buttonRead = digitalRead(SAI_BUTTONPIN);
  int dnd_buttonRead = digitalRead(DND_BUTTONPIN);

  if(sleep_buttonRead == HIGH)
  {
    Serial.println("Sleep");
    changeLightState(&sleep_isLit, 'n');
    delay(1000);
  }

  if(andrew_buttonRead == HIGH)
  {
    Serial.println("Andrew");
    changeLightState(&andrew_isLit, 'a');
    delay(1000);
  }
  
  if(sai_buttonRead == HIGH)
  {
    Serial.println("Sai");
    changeLightState(&sai_isLit, 's');
    delay(1000);
  }
  
  if(dnd_buttonRead == HIGH)
  {
    Serial.println("DND");
    changeLightState(&dnd_isLit, 'd');
    delay(1000);
  }

}

void changeLightState(bool *isLit, char buttonID)
{
  if(!*isLit)
  {
    char setP[3];
    setP[0] = 's';
    setP[1] = buttonID;
    setP[2] = null;
    stanky.send(RECEIVER, setP, strlen(setP));
    *isLit = true;
  } 
  else if(*isLit)
  {
    char resetP[3] = {'r', buttonID, null};
    stanky.send(RECEIVER, resetP, strlen(resetP));
    *isLit = false;
  }

  stanky.receiveDone();
}
