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

#define zeroCode 95 // 
#define OLED_ADDR   0x3C

SoftwareSerial mySerial(10, 11); // RX, TX
Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int read_P1, read_P2, read_P3; // all dynamic input
int menuState = 1; //menu basic states
int menuLastState;

int allSimple,allDouble; //number of simple and double shoots

char** arraySimple;
char** arrayDouble_F;
char** arrayDouble_S;
int arraySimpleINT;
int** arrayDoubleINT;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("HC-12 has been set");
 
  delay(500);
  
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
  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //display.print("Press Enter");
  functionLauncher();

}
void functionLauncher(){ //like main() do every side function a row
  //menuChannels(); //set field == set frequency                    --
  menuNumOfSimple(); //set "How many simple shoot have"             -OK-
  menuSimple();  // choose the Simple shoots machines               -OK-
  menuNumOfDouble();  //set "How many double shoot have"            -OK-
  menuDouble();  //chosse the Double shoots paars                   -OK-
  compilledMenu();
}
void loop(){
  /*read_P1 = digitalRead(P1);
  read_P2 = digitalRead(P2);
  read_P3 = digitalRead(P3);
  if(read_P2 == 0){
    display.clearDisplay();
    delay(250);
    menuLastState = menuState;
    menuState = menuState+1;
    menu_S();
    
    
  }
  if(read_P1 == 0){
    display.clearDisplay();
    delay(250);
    menuLastState = menuState;
    menuState--;
    menu_S();
    
    
  }*/
 /*if(read_P1 == 0){
   switch(menuState){
    case 0 :
     TXCODE = 94;
      break;
    case 1 :
      TXCODE = 93;
      break;
    case 2 :
      TXCODE = 91;
      break;
   case 3 :
      TXCODE = 87;
      break;
   case 4 :
      TXCODE = 79;
      break;
   default: 
       TXCODE = 94;
  }
  TXTOSEND = TXCODE;
  Serial.print(TXCODE, BIN);
  mySerial.print(TXTOSEND);
  delay(2000);
  TXTOSEND = zeroCode;
  Serial.print(zeroCode, BIN);
  mySerial.print(TXTOSEND);
   feedback=mySerial.read();
    Serial.println(feedback);
    if (feedback >= 1)
    {
     ammos[menuState] = feedback;
    Serial.println("Completed"); // feedback to serial port
    digitalWrite(F1, HIGH);    // Ligjt Led Back on Transmitter
    delay(100);
    digitalWrite(F1, LOW);  
    }
    else if(feedback == 0){
      ammos[menuState] = 0;
      Serial.println("Out Of Ammo");}
    else if(feedback == -1){
      
      }  
      }*/
  }

void menuNumOfSimple(){
  Serial.print("\nmenuNumOfSimple: in | ");
  int stylePush = 10;
  allSimple = 0;
  boolean finished = true; //while operator
  sytleOfNumMenu("S:",true,allSimple,stylePush);
  while(finished){ //unlimted + numeric counter =/= enter
      while(digitalRead(P1) == 1 && digitalRead(P2) == 1 && digitalRead(P3) == 1){} //to waiting an input data
      if(digitalRead(P2) == 0){
        allSimple++;  //up
        if(allSimple < 10){stylePush = 10;}
          else{stylePush = 0;}
        sytleOfNumMenu(NULL,false,allSimple,stylePush);
        } 
      else if(digitalRead(P3) == 0){finished = false;} //close the counter list 
      else if(digitalRead(P1) == 0){
        if(allSimple > 0){allSimple--;} //down
        if(allSimple < 10){stylePush = 10;}
          else{stylePush = 0;}
        sytleOfNumMenu(NULL,false,allSimple,stylePush);
        }
      delay(500); // delay to human button pressing reaction time
      Serial.print(stylePush);
  }
  Serial.print(allSimple);
  //---creating the dynamic "char" Simple array---
  arraySimple = new char*[allSimple]; //generate the dynamic array
  arraySimpleINT = new int[allSimple]; //to intger format
  //delete [] arraySimple; //delete the old one
  Serial.print("--OK--");
}

void menuNumOfDouble(){
  Serial.print("\nmenuNumOfDouble: in | ");
  int stylePush = 10;
  allDouble = 0;
  boolean finished = true; //while operator
  sytleOfNumMenu("D:",true,allDouble,stylePush);
  while(finished){ //unlimted + numeric counter =/= enter
      while(digitalRead(P1) == 1 && digitalRead(P2) == 1 && digitalRead(P3) == 1){} //to waiting an input data
      if(digitalRead(P2) == 0){
        allDouble++;  //up
        if(allDouble < 10){stylePush = 10;}
          else{stylePush = 0;}
        sytleOfNumMenu(NULL,false,allDouble,stylePush);
        } 
      else if(digitalRead(P3) == 0){finished = false;} //close the counter list 
      else if(digitalRead(P1) == 0){
        if(allDouble > 0){allDouble--;} //down
        if(allDouble < 10){stylePush = 10;}
          else{stylePush = 0;}
        sytleOfNumMenu(NULL,false,allDouble,stylePush);
        }
      delay(500); // delay to human button pressing reaction time
  }
  Serial.print(allDouble);
  //---creating the dynamic "char" Simple array---
  arrayDouble_F = new char*[allDouble];//generate the dynamic array
  arrayDouble_S = new char*[allDouble];//generate the dynamic array
  arrayDoubleINT = new int*[allDouble];
  for(int i=0; i < allDouble; i++){
    arrayDoubleINT[i] = new int[2];}
  //delete [] arrayDouble; //delete the old one
  Serial.print("--OK--");
}
void sytleOfNumMenu(char* typeOfMenu, boolean allScreen, int menuNumber, int push){
  if(!allScreen){
    display.drawRect(65,0,62,32,BLACK);
    display.fillRect(65,0,62,32,BLACK);
    display.setCursor(75+push,5);
    display.print(menuNumber);
    }
  else{
    display.setTextSize(3);
    display.clearDisplay();
    display.setCursor(20,5);
    display.print(typeOfMenu);
    display.setCursor(75+push,5);
    display.print(menuNumber);
    }
  display.display();
}
void menuSimple(){
  Serial.print("\nmenuSimple: in | "); 
  int finished = 0; // a num "Which menu point already has set"
  int _finished = finished; //finished's temp
  int started = 0; // a num "Which menu point under choosing
  display.setTextSize(3);
  while(finished < allSimple){ //while all simple menu point not set
    display.clearDisplay();
    display.setCursor(55,5); 
    display.print("A"); //optimising first displaying object (to not enter the switch constanly)
    display.display();
    menuState = 1; //reset menu scrolling
    started++; // first choosing is started
    while(finished < started){ // while started menu point under choosing
      while(digitalRead(P1) == 1 && digitalRead(P2) == 1 && digitalRead(P3) == 1){} //to waiting an input data
      if(digitalRead(P2) == 0){menuState++;} //scrolling in the menu down
      else if(digitalRead(P3) == 0){_finished++;} //to choose that machine
      else if(digitalRead(P1) == 0){menuState--;} //scrolling in the menu up
      delay(500); // delay to human button pressing reaction time
      if(_finished == finished){  //if the user chose to scrolling
        Serial.print(menuState); 
        menuSWITCH(0); //displaying next machines id
        display.display();
        }
      else{ // if the user chose select that machine
        arraySimple[finished] = menuReturner(); //save machine id to an array => future create the final menu from these
        Serial.print(arraySimple[finished]);
        finished = _finished; //jump to the next
        }
      }
  }
}
void menuDouble(){
  Serial.print("\nmenuDouble: in | "); 
  int finished = 0; // a num "Which menu point already has set"
  int _finished = finished; //finished's temp
  int started = 0; // a num "Which menu point under choosing
  display.setTextSize(3);
  while(finished < allDouble){ //while all simple menu point not set
    boolean firstChoose = true;
    display.clearDisplay();
    display.setCursor(20,5); 
    display.print("A"); //optimising first displaying object (to not enter the switch constanly)
    display.display();
    menuState = 1; //reset menu scrolling
    started++; // first choosing is started
    while(finished < started){ // while started menu point under choosing
        while(digitalRead(P1) == 1 && digitalRead(P2) == 1 && digitalRead(P3) == 1){} //to waiting an input data
        if(digitalRead(P2) == 0){
          menuState++;
          if(!firstChoose)if(menuState == arrayDoubleINT[finished][0])menuState++;} //scrolling in the menu down and skip same machine at 2nd
        else if(digitalRead(P3) == 0){
          if(firstChoose){
            _finished++; 
            arrayDouble_F[finished] = menuReturner(); //save 2nd machine id to an array => future create the final menu from these
            arrayDoubleINT[finished][0] = menuState; //int array to storing first machine id
            Serial.print(arrayDouble_F[finished]);
            menuState = 1;
            }
            else{
              _finished++;
              arrayDouble_S[finished] = menuReturner(); //save 2nd machine id to an array => future create the final menu from these
              arrayDoubleINT[finished][1] = menuState; //int array to storing first machine id
              Serial.print(arrayDouble_S[finished]);
              }
          } //to choose that machine
        else if(digitalRead(P1) == 0){
          menuState--;
          if(!firstChoose)if(menuState == arrayDoubleINT[finished][0])menuState++;} //scrolling in the menu up and skip same machine at 2nd
        delay(500); // delay to human button pressing reaction time
        if(_finished == finished && firstChoose){  //if the user chose to scrolling
          Serial.print("--if1--"); 
          menuSWITCH(1); //displaying 1st machines ids
          display.display();
          }
        else if(_finished == finished && !firstChoose){
          Serial.print("--if2--"); 
          menuSWITCH(2); //displaying 2nd machines ids
          display.display();
          }
        else if(_finished != finished && firstChoose){
          firstChoose = false;
          _finished--;
          display.setCursor(90,5);
          if(menuState == arrayDoubleINT[finished][0]){
            display.print("B");
            }
          else{display.print("A");}
          display.drawLine(55,16,65,16,WHITE);
          display.display();
        }
        else if(_finished != finished && !firstChoose){
          Serial.print("--if else--");
          finished = _finished; //jump to the next
        }
      
     }
  }
  
  //menuSWITCH(1);
  
  //display.drawLine(16,55,16,65);
  //menuSWITCH(2);
  

}
void menuSWITCH(int NumOfElement){
  display.setTextSize(3);
  if(NumOfElement == 0){
    display.setCursor(55,5);
    display.clearDisplay();
    }
   else if(NumOfElement == 1){
    display.setCursor(20,5);
    display.clearDisplay();
    }
   else if(NumOfElement == 2){
    display.drawRect(75,0,62,32,BLACK);
    display.fillRect(75,0,62,32,BLACK);
    display.setCursor(90,5);}

    if(menuState > 10){menuState = 1;}
    if(menuState < 1){menuState = 10;}

    switch(menuState){
      case 1 :
        display.print("A");
        break;
      case 2 :
        display.print("B");
        break;
      case 3 :
        display.print("C");
        break;
      case 4 :
        display.print("D");
        break;
      case 5 :
        display.print("E");
        break;
      case 6 :
        display.print("F");
        break;
      case 7 :
        display.print("G");
        break;
      case 8 :
        display.print("H");
        break;
      case 9 :
        display.print("I");
        break;
      case 10 :
        display.print("J");
    }
    
}
char* menuReturner(){
  switch(menuState){
      case 1 :
              return("A");
      case 2 :
              return("B");
      case 3 :
              return("C");
      case 4 :
              return("D");
      case 5 :
              return("E");
      case 6 :
              return("F");
      case 7 :
              return("G");
      case 8 :
              return("H");
      case 9 :
              return("I");
      case 10 :
              return("J");
    }
  }

void compilledMenu(){}
/*void writeAmmo(){
  display.setTextSize(2);
  display.setCursor(90,10);
  if(ammos[menuState] == NULL){
    display.print("---");}
  else if(ammos[menuState] == -1){
    display.setTextSize(1);
    display.setCursor(75,12);
    display.print("Hiba: B1");}
  else if(ammos[menuState] == -2){
    display.setTextSize(1);
    display.setCursor(75,12);
    display.print("Hiba: B2");}
  else if(ammos[menuState] == -3){
    display.setTextSize(1);
    //display.clearDisplay();
    display.setCursor(75,7);
    display.print("Gyenge");
    display.setCursor(75,16);
    display.print("telep");}
  else if(ammos[menuState] <= 9){
    display.print("00"+String(ammos[menuState]));}
  else if(ammos[menuState] <= 99){
    display.print("0"+String(ammos[menuState]));}
  else display.print(ammos[menuState]);
  }*/
