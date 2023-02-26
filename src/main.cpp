#include <Arduino.h> //Declared weak in Arduino.h to allow user redefinitions.
#include "MR72.h" //#include "MR72.h" Front and Rear sensor
#include "Smoothed.h" //#include "Smoothed.h" To Smoothing Inconsistant Value
#define MRUART72 Serial1 //#define MR72 Serial1 RX:TX(0,1)
MR72 Mr72(MRUART72); //Define MR72 to class Mr72 

void setup() {
  delay(5000); //Delay 5 second before procced to the next line
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  Mr72.begin(115200); // set the data rate for the TelemSerial port

}
void loop() {

  Mr72.IncomingNewMessage(); //Call IncomingNewMessage From Mr72

  Mr72.Distance_Uart_sl; //Left Value
  Mr72.Distance_Uart_sc; //Center Value
  Mr72.Distance_Uart_sr; //Right Value
}

