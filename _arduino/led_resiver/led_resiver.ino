/*HC-12 Moule Transciever Same code is for Transmitter and Receiver, setup through A4 Input
  Tested on Arduino Nano
  This SW is for Transmitter & Receiver, the choise is done by reading A4 & A5 inputs (analog inputs)
  A4=1=> TX, A4=0=>RX
   A5=1=>LATCH, A5=0=>Momentary this feature doesn't work in this version so this is latch only!

   NANO connections to HC-12 As follows:
   ARDUINO NANO                                        HC-12-MODULE
   _________________________________________________________________________
   5V            -> 1N4148 Anode, 1N4148 catode ->     HC-12/Pin-1 (VCC)
   GND                  ------->                          GND
   D11                  ------->                          RXD
   D10                  ------->                          TXD

   NANO (TX MODULE)                              NANO (RX MODULE)
   ______________________                    ________________________________
   D2 --> SW --> GND (SW1)                   A4 --> GND
   D3 --> SW --> GND (SW2)                   D2 --> OUTPUT 1
   D4 --> SW --> GND (SW3)                   D3 --> OUTPUT 2
   D5 --> SW --> GND (SW4)                   D4 --> OUTPUT 3
                                             D5 --> OUTPUT 4
*/
#define P1 2
#define P2 3
#define P3 4
#define P4 5

#define D1 3
#define D2 4
#define D3 5
#define D4 6

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
char latch; // , if latch=1 then Latch
int read_A4, read_A5, i, read_P1, read_P2, read_P3, read_P4;
int TX_CODE, RX_READ, mode, TX_CODE_OLD; //if mode= 1 then TX
int RP1, RP2, RP3, RP4;
volatile byte SW1 = 0, SW2 = 0, SW3 = 0, SW4 = 0;
boolean S1, S2, S3, S4;
byte TEST;
char s, TXTOSEND;
int FLAG = 1;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Hello HC-12 TX and RX SW, A4-Low=TX A4-HIGH=RX A5-LOW=Momemtary A5-HIGH=LATCH");
  // mySerial.println("Hello HC-12 TX and RX SW, A4-Low=TX A4-HIGH=RX A5-LOW=Momemtary A5-HIGH=LATCH");
  // This is for internal pull ups resistors on A4 and A5 Inputs
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  delay(500);
  read_A4 = analogRead(A5);
  read_A5 = 0;
  //read_A5 = analogRead(A5);
  if ((read_A4) > 500) {
    mode = 1;
    Serial.println("TX Mode");
  }
  else
  {
    mode = 0;
    Serial.println("RX Mode");
  }

  if ((read_A5) > 500) {
    latch = 1;
    Serial.println("LATCH Mode");
  }
  else
  {
    latch = 0;
    Serial.println("Momentary Mode");
  }
//mode=0;
  if (mode == 1) // TX
  {
    pinMode(P1, INPUT);           // set pin to input
    digitalWrite(P1, HIGH);       // turn on pullup resistors
    pinMode(P2, INPUT);           // set pin to input
    digitalWrite(P2, HIGH);       // turn on pullup resistors
    pinMode(P3, INPUT);           // set pin to input
    digitalWrite(P3, HIGH);       // turn on pullup resistors
    pinMode(P4, INPUT);           // set pin to input
    digitalWrite(P4, HIGH);       // turn on pullup resistors
  }
  else
  {
    pinMode(D1, OUTPUT);           // set pin to input
    digitalWrite(D1, LOW);       // turn on pullup resistors
    pinMode(D2, OUTPUT);           // set pin to input
    digitalWrite(D2, LOW);       // turn on pullup resistors
    pinMode(D3, OUTPUT);           // set pin to input
    digitalWrite(D3, LOW);       // turn on pullup resistors
    pinMode(D4, OUTPUT);           // set pin to input
    digitalWrite(D4, LOW);       // turn on pullup resistors

  }

}

void loop() {

  if (mode == 1) //TX Mode
  {
    TX_MODE();
  }
  else
  {
    RX_MODE();
  }

}

void TX_MODE()
{
  while (1)
  {
    {
      read_P1 = digitalRead(P1);
      read_P2 = digitalRead(P2);
      read_P3 = digitalRead(P3);
      read_P4 = digitalRead(P4);

      TX_CODE = 64 + (read_P1 * 1) + (read_P2 * 2) + (read_P3 * 4) + (read_P4 * 8); // converting TX to one char
      TXTOSEND = TX_CODE; // conver to char for sending OK
      if (FLAG == 0) // skip first time on FLAG=1
      {
        if (TX_CODE != TX_CODE_OLD)  // transmit only on change
        {
          Serial.print(TX_CODE, BIN);
          mySerial.print(TXTOSEND);
        }
        delay(100);
        TX_CODE_OLD = TX_CODE;
      }
      else FLAG = 0;
    }
  }
}

void RX_MODE()
{
  while (1)
  
  {
    if (mySerial.available()) {
      // read the incoming byte:
      
      RX_READ = mySerial.read();
      TEST = RX_READ;
      RP1 = (TEST) & (1);
      RP2 = (TEST) & (2);
      RP3 = (TEST) & (4);
      RP4 = (TEST) & (8);

      mySerial.flush();

      // When button is pressed (logic 0) toggling the apropriate LED
      if (RP1 == 0) SW1 = ~(SW1);
      if (RP2 == 0) SW2 = ~(SW2);
      if (RP3 == 0) SW3 = ~(SW3);
      if (RP4 == 0) SW4 = ~(SW4);

      //convert to bit for Digital-Out from Arduino
      S1 = SW1 & 1;
      S2 = SW2 & 1;
      S3 = SW3 & 1;
      S4 = SW4 & 1;

      digitalWrite(D1, S1);
      digitalWrite(D2, S2);
      digitalWrite(D3, S3);
      digitalWrite(D4, S4);

      delay(100);
      
      Serial.print("S1=");
      Serial.print(SW1);
      Serial.print("  S2=");
      Serial.print(SW2);
      Serial.print("  S3=");
      Serial.print(SW3);
      Serial.print("  S4=");
      Serial.println(SW4);

    }

  }
}
