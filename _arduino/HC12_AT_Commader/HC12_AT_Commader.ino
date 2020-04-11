/*HC-12 AT Commander
 *By Németh Bálint
 *Last modification: 2019.05.05.
 */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX pins on arduino
void setup() {
    Serial.begin(9600); //default baud rate
    Serial.println("AT commands:");
    mySerial.begin(9600);

}

void loop() {
  if(mySerial.available()){
    Serial.write(mySerial.read());  
    }
  if(Serial.available()){
    mySerial.write(Serial.read());
    }
}
