/*By Németh Bálint - Version 1.1
 *Last modification: 2019.05.05
 * NANO connections to HC-12 As follows:
 * ARDUINO NANO                                        HC-12-MODULE
 * _________________________________________________________________________
 * 5V            -> 1N4148 Anode, 1N4148 catode ->     HC-12/Pin-1 (VCC)
 * GND                  ------->                          GND
 * D11                  ------->                          RXD 
 * D10                  ------->                          TXD
 * 
 * NANO (TX SERVER MODULE)                          
 * ______________________                    
 * D2 --> SW --> GND (SW1)                  
 * D3 --> SW --> GND (SW2)                  
 * D4 --> SW --> GND (SW3)                  
 * D5 --> SW --> GND (SW4)                  
 * D6 --> SW --> GND (LED1)                                       
 * D10 --> RX (OUT)
 * D11 --> TX (IN)
 * A5 --> GND (PHYSICAL SWITCH)
 */
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define P1 2 // INPUTS
#define P2 3
#define P3 4
#define F1 5

#define zeroCode 79 // 
#define OLED_ADDR   0x3C

SoftwareSerial mySerial(10, 11); // RX, TX
Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int read_A5, i, read_P1, read_P2, read_P3, read_P4;
int TX_CODE, RX_READ, TX_CODE_OLD; //if mode= 1 then TX
char cliens [5] = {'1','2','3','4','5'};
char TXTOSEND;
int feedback = 0;
int FLAG=1,latch_logic;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Module has been set");
 
  delay(500);
  
    pinMode(A5, INPUT_PULLUP);
    read_A5 = analogRead(A5);
    pinMode(P1, INPUT);           // set pin to input
    digitalWrite(P1, HIGH);       // turn on pullup resistors
    pinMode(P2, INPUT);           // set pin to input
    digitalWrite(P2, HIGH);       // turn on pullup resistors
    pinMode(P3, INPUT);           // set pin to input
    digitalWrite(P3, HIGH);       // turn on pullup resistors
    pinMode(F1, OUTPUT);           // set pin to input
    digitalWrite(F1, LOW);
        
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setRotation(1);
  menu();
  display.display();
  //display.drawPixel(0, 0, WHITE);
}
 
void loop() {
    TX_MODE();

 
}
 
void TX_MODE(){
  while (1)
    { 
      read_P1 = digitalRead(P1);
      read_P2 = digitalRead(P2);
      read_P3 = digitalRead(P3);
      read_P4 = 1; //digitalRead(P4);
      
      //display.setCursor(10,10);
      display.setTextColor(BLACK); 
      if(read_P1 == 0){
        display.fillRect(9,0,12,20,WHITE);
        display.drawRect(10,0,10,20,WHITE);
        display.setCursor(10,2);
        display.print("A");
       }
       if(read_P2 == 0) {
        display.fillRect(9,25,12,20,WHITE);
        display.drawRect(10,25,10,20,WHITE);
        display.setCursor(10,27);
        display.print("B");
        }
       if(read_P3 == 0) {
        display.fillRect(9,50,12,20,WHITE);
        display.drawRect(10,50,10,20,WHITE);
        display.setCursor(10,52); 
        display.print("C");
        }
       if(read_P4 == 0) {
        display.fillRect(9,75,12,20,WHITE);
        display.drawRect(10,75,10,20,WHITE);
        display.setCursor(10,77);
        display.print("D");
        }
       display.display();
      //if(read_P1 == 0 ){digitalWrite(F1, HIGH);}
      
      TX_CODE = 64 + (read_P1 * 1) + (read_P2 * 2) + (read_P3 * 4) + (1 * 8); // converting TX to one char
      TXTOSEND = TX_CODE; // conver to char for sending OK
      if (FLAG==0) // skip first time on FLAG=1
          {
         /* if (latch==1)
              {  
              if ((TX_CODE != TX_CODE_OLD)&&(latch_logic==1))  // transmit only on change
              {
                Serial.print(TX_CODE, BIN);
                mySerial.print(TXTOSEND);
              }
              delay(100);
              TX_CODE_OLD = TX_CODE;
              }*/

                if(read_P1 != 1 || read_P2 != 1 || read_P3 !=1 || read_P4 !=1 ){
                Serial.print(TX_CODE, BIN);
                mySerial.print(TXTOSEND);
                delay(2000);
                TXTOSEND = zeroCode;
                Serial.print(zeroCode, BIN);
                mySerial.print(TXTOSEND);

                /*delay(2500);
                if((TX_CODE != TX_CODE_OLD)&&(latch_logic==1)){
                  Serial.print(TX_CODE, BIN);
                  mySerial.print(TXTOSEND);
                  TX_CODE_OLD = TX_CODE;
                  }*/
                
              }
              
          }
      else FLAG=0;
      int clearscr = read_P1+read_P2+read_P3+read_P4;
      if(clearscr < 4){
        display.clearDisplay();
         menu();
         Serial.println("Menu refreshed");
        display.display();
        } 
    
    if(read_P1 == 1 ){digitalWrite(F1, LOW);}
    feedback=mySerial.read();
    Serial.println(feedback);
    if (feedback=='R')
    {
    Serial.println("Data_Sent_OK"); // feedback to serial port
    digitalWrite(F1, HIGH);    // Ligjt Led Back on Transmitter
    delay(100);
    digitalWrite(F1, LOW);  
    }
}}

void menu(){
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,2);
    display.print("A");
    display.setCursor(10,27);
    display.print("B");
    display.setCursor(10,52);
    display.print("C");
    display.setCursor(10,77);
    display.print("D");
  
  }
