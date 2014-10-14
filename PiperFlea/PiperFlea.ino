/*********************************************************************
 * rFlea_Arduino library v1.0
 * 5 october 2014
 *
 * This is an example sketch for the rFlea ANT+ arduino compatible
 * 
 * This example is known as full duplex communication (TxRx). It refers 
 * to the ability of sending and receiving data to the phone or tablet,
 * or any other rFlea is set up correctly (Beacon_listener example).
 *
 * rFlea are prototyping boards with ultra low power wireless ANT:
 * Refer to http://www.thisisant.com/ for more information
 * 
 * rFlea are based in ATmega 328, same as Arduino Pro Mini or Arduino
 * LilyPad. Use those boards to flash rFlea in the tools menu.
 * 
 * Written by Jordi Solsona. September 2014. Email: Jordisb@kth.se
 * 
 * This example shows how to connect to a Android mobile phone or tablet
 * compatible with ANT+
 * Refer to http://www.thisisant.com/consumer/ant-101/ant-in-phones
 * 
 * The rFlea webapp must be downloded and installed in the device.
 * 
 *********************************************************************/

#include <rFlea_Arduino.h>
#include <EEPROM.h>
#define COMMON_ANODE

//rFlea Object and constructor.
rFlea_Arduino rflea = rFlea_Arduino();
unsigned int serialNumber;

int led = 13;
int redPin = 11;
int greenPin = 3;
int bluePin = 10;

void setup() {
  //rFlea object uses Serial and need to be at 57600
  //don't change it or the wireless will not work
  //you can still use it as debug normally.
  Serial.begin(57600);

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     

  //Set digital and analog pins
  pinMode(12,OUTPUT);

  //Set pullup resistors
  digitalWrite(12, LOW);  
  
  //turn the RGB LED off  
  analogWrite(greenPin,255); //green
  analogWrite(bluePin,255); //blue
  analogWrite(redPin,0); //red 0 = full strength

  //Registrer the functions that will be called to help syncronisation,
  // low power and receive data.
  rflea.register_onSync(onSync);
  rflea.register_onMessageSensorRx(onMessageSensorRx);

  //Set our SENSOR_RX channel. SENSOR_RX can transmit and receive
  rflea.rFlea_profile(SENSOR_RX);
  
  //Get the unique serial number from this rFlea and print it
  serialNumber=rflea.my_serial_number();
  Serial.println(" ");
  Serial.print("S/N: ");
  Serial.println(serialNumber);
  
  //Reset and Initialize the ANT+.
  rflea.init();

  //Connect!!
  rflea.connect(SENSOR_RX);
}
void loop() {
  //Update rFlea every loop.
  rflea.update();

}

void puffEvent() {
  analogWrite(redPin, 0);

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    setColor(fadeValue, 0, 0);
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
}

//In case we are a Sensor, this function will be called
// every time we are ready to send next message. Use the
// function rFlea.send(byte[]) to send the 8 bytes of data
void onSync(){
  byte  message[8];
  message[0] = unsigned(analogRead(A4)/4);//arduino is 10 bits, 1byte is 8 bits.
  //Dividing by 4 we remove 2 bits
  message[1] = unsigned(analogRead(A5)/4);
  message[2] = 0;//digitalRead(3);
  message[3] = 0;//digitalRead(10);
  message[4] = 0;//digitalRead(11);
  message[5] = 0;//digitalRead(12);
  message[6] = 0; //Empty
  message[7] = 0; //Empty
  rflea.send(SENSOR_RX,message);
}

void setColor(int red, int green, int blue) {
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue); 
}


//This Function will be called everytime we receive something
void onMessageSensorRx(byte* message){ 

  if (message[0]==1) puffEvent();
}
