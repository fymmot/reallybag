/*********************************************************************
This is an example sketch for the rFlea Stiva ANT+ arduino compatible

rFlea Stiva are prototyping boards with ultra low power wireless ANT:
  Refer to http://www.thisisant.com/ for more information

rFlea Stiva are based in ATmega 328, same as Arduino Pro Mini or Arduino
LilyPad. Use those boards to flash rFlea Stiva in the tools menu.

Written by Jordi Solsona. September 2014. Email: Jordisb@kth.se

This example shows how to connect to a Android mobile phone or tablet
compatible with ANT+
  Refer to http://www.thisisant.com/consumer/ant-101/ant-in-phones

The rFlea webapp must be downloded and installed in the device.

This example is known as full duplex communication (TxRx). It refers 
to the ability of sending and receiving data to the phone or tablet.
*********************************************************************/

#include <rFlea_Stiva.h>
#include <EEPROM.h>

//rFlea Object and constructor.
rFlea_Stiva rflea = rFlea_Stiva();

void setup() {
  //rFlea object uses Serial and need to be at 57600
  //don't change it or the wireless will not work
  //you can still use it as debug normally.
  Serial.begin(57600);
  
  //Registrer the functions that will be called to help syncronisation,
  // low power and receive data.
  rflea.register_onSync(onSync);
  rflea.register_onMessage(onMessage);
  
  //Reset and Initialize the ANT+.
  rflea.init();
  
  //Connect!!
  rflea.connect();
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
  message[0] = random(0, 255);
  message[1] = random(0, 255);
  message[2] = random(0, 1);
  message[3] = random(0, 1);
  message[4] = random(0, 1);
  message[5] = random(0, 1);
  message[6] = 10;
  message[7] = 10;
  rflea.send(message);
}


//This Function will be called everytime we receive something
void onMessage(byte* message){


}












