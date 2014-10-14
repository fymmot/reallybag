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

//rFlea Object and constructor.
rFlea_Arduino rflea = rFlea_Arduino();
unsigned int serialNumber;

int led = 13;

void setup() {
  //rFlea object uses Serial and need to be at 57600
  //don't change it or the wireless will not work
  //you can still use it as debug normally.
  Serial.begin(57600);

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     

  //Set digital and analog pins
//  pinMode(3,OUTPUT);
//  pinMode(10,OUTPUT);
//  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);

  //Set pullup resistors
//  digitalWrite(3, HIGH);  
//  digitalWrite(10, LOW); 
//  digitalWrite(11, LOW); 
  digitalWrite(12, LOW);  
  
  analogWrite(3,128); //green
  analogWrite(10,128); //blue
  analogWrite(11,255); //red

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


//This Function will be called everytime we receive something
void onMessageSensorRx(byte* message){ 
  //Change to true if you want to print the data received 
  if(true){
    Serial.println("");
    Serial.print("Rx from: ");
    for(int i=0;i<8;i++){
      Serial.print(message[i],HEX);    
      Serial.print(" ");
    }  
    Serial.println("");
  }

  if (message[0]==0)
  digitalWrite(led, LOW); //If the first byte is 0 then LED off
  else 
    digitalWrite(led, HIGH); //If the first byte is other than 0 then LED on
}
