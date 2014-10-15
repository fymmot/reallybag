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
int redPin = 11;
int bluePin = 3;
int greenPin = 10;

int basePressure;
int maxPressure;
int minPressure;

//ACTIONS
const int PUFF = 1;
const int PUFFPUFF = 2;
const int SIP = 3;
const int SIPSIP = 4;

//STATES
const int NEUTRAL = 0;
const int BLOW = 1;
const int SUCK = 2;

int event;
double volume = 0.5;
double volumeChangeRatio = 0.002;

int noiseMargin = 2;
int state = NEUTRAL;
int previousAction = 0;
int maxSipDuration = 400;
int maxPuffDuration = 400;
int endActionTime = 400;	// how long to wait for the next action for combining actions
char valueBuffer[2] = {
  'n','n'}; // how many relevant pressure readings in a row before we care
long startTime;
long actionStart;
int currentPressure;

//DEBUG
int counter = 0;
boolean ledOn = false; 

//////////////////////////////////////////////////////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  //rFlea object uses Serial and need to be at 57600
  //don't change it or the wireless will not work
  //you can still use it as debug normally.
  Serial.begin(57600);

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     

  //Set pullup resistors
  digitalWrite(12, HIGH);

  setColor(255,255,255);


  //Register the functions that will be called to help syncronisation,
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

  setupBagPipe(); 
}

void setupBagPipe() {
  int pressure = unsigned(analogRead(A5)/4);
  basePressure = pressure; //measure the ambient air pressure in the very beginning
  maxPressure = basePressure + 5;
  minPressure = basePressure - 5; 
}

//////////////////////////////////////////////////////////////////////////////////////////////
// LOOP
//////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  //Update rFlea every loop.
  rflea.update();
  
//  if (counter%100 < 50) digitalWrite(led,HIGH);
//  else digitalWrite(led,LOW);
//  counter++;
  
  currentPressure = unsigned(analogRead(A5)/4);

  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FAKE START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // REMOVE THIS IF YOU WANT ACTUAL VALUES FROM THE PRESSURE SENSOR!!!
  fake();
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< FAKE END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  
  int pressure = currentPressure;

  valueBuffer[0] = valueBuffer[1]; //this is the shift!

  if(pressure > basePressure + noiseMargin){
    valueBuffer[1] = 'b';
  }
  else if(pressure <= basePressure + noiseMargin && pressure >= basePressure - noiseMargin) {
    valueBuffer[1] = 'n';
  }
  else if(pressure < basePressure - noiseMargin) {
    valueBuffer[1] = 's';
  }
  for (int i = 1; i > 0; i--) {
    if(valueBuffer[i] != valueBuffer[i-1]){
      return;
    }
  };
  processPressure(pressure); //called only if we got nice values and not just noise  
  checkEventsAndFeedback();
}

void checkEventsAndFeedback() {
  if (millis() > actionStart + endActionTime) { //we ran out of time already
    if (previousAction !=0) { //we already got something happening
      //send the action that has been performed to the phone to trigger audio control
      sendEvent(previousAction);
      setColor(255,255,255); //turn off the lights
      previousAction = 0; //reset the action to neutral
    }
  }
  else { //didn't run out of time yet
    int fadeColorValue = (int) ((((float) millis() - actionStart) / endActionTime) * 255);
    switch (previousAction) {
    case PUFF:
      setColor(fadeColorValue,255,255);
      break;
    case PUFFPUFF:
      setColor(fadeColorValue,255,255);
      break;
    case SIP:
      setColor(255,255,fadeColorValue);
      break;
    case SIPSIP:
      setColor(255,255,fadeColorValue);
      break;
    }    
  }
}

void processPressure(int pressure) {
  //// If we're here, we've gotten some good values in a row (i.e. not noise)
  double intensity;
  int fadeColorValue;
  if (pressure > maxPressure) maxPressure = pressure;
  else if (pressure < minPressure) minPressure = pressure;

  switch (state) {

  case NEUTRAL:
    if (pressure > basePressure + noiseMargin) {
      blowStart();
    } 
    else if (pressure < basePressure - noiseMargin) {
      suckStart();
    }
    break;

    ///////////////////////////////////////////////////////////////////////////////////////

  case BLOW:
    if (millis() - startTime > maxPuffDuration) { //long blow
      intensity = (double) (pressure - basePressure) / (maxPressure - basePressure);
      volume = volume + intensity * volumeChangeRatio;
      if (volume > 1) volume = 1;
      fadeColorValue = (int) ((1 - volume) * 255);
      setColor(fadeColorValue,255,255);
    }
    if (pressure <= basePressure + noiseMargin && pressure >= basePressure - noiseMargin) {
      blowStop();
      neutralStart();
    } 
    else if (pressure < basePressure - noiseMargin) {
      blowStop();
      suckStart();
    }
    break;

    ///////////////////////////////////////////////////////////////////////////////////////

  case SUCK:
    if (millis() - startTime > maxSipDuration) { //long suck
      intensity = (double) (basePressure - pressure) / (basePressure - minPressure);
      volume = volume - intensity * volumeChangeRatio;
      if (volume < 0) volume = 0;
      fadeColorValue = (int) ((1 - volume) * 255);
      setColor(255,255,fadeColorValue);
    }
    if (pressure >= basePressure - noiseMargin && pressure <= basePressure + noiseMargin) {
      suckStop();
      neutralStart();
    } 
    else if (pressure > basePressure + noiseMargin) {
      suckStop();
      blowStart();
    }
    break;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// MESSAGING
//////////////////////////////////////////////////////////////////////////////////////////////

//In case we are a Sensor, this function will be called
// every time we are ready to send next message. Use the
// function rFlea.send(byte[]) to send the 8 bytes of data
void onSync(){
  byte  message[8];
  message[0] = (int) (volume * 100);//unsigned(analogRead(A4)/4);//arduino is 10 bits, 1byte is 8 bits.
  //Dividing by 4 we remove 2 bits
  message[1] = event;////unsigned(analogRead(A5)/4);
  message[2] = 0;//digitalRead(3);
  message[3] = 0;//digitalRead(10);
  message[4] = 0;//digitalRead(11);
  message[5] = 0;//digitalRead(12);
  message[6] = 0; //Empty
  message[7] = currentPressure; //Empty
  rflea.send(SENSOR_RX,message);

  event = 0; // do not send the same shit twice
}


//This Function will be called everytime we receive something
void onMessageSensorRx(byte* message){   
  //Change to true if you want to print the data received 
}

//////////////////////////////////////////////////////////////////////////////////////////////
// STATES
//////////////////////////////////////////////////////////////////////////////////////////////

void neutralStart() {
  state = NEUTRAL;
}

void blowStart() {
  state = BLOW;
  startTime = millis();
}

void blowStop() {
  int duration = millis() - startTime;
  if (duration < maxPuffDuration) {
    if (previousAction == PUFF) setPreviousAction(PUFFPUFF);
    else setPreviousAction(PUFF);
  }
  else {
    // lights out
    setColor(255,255,255);
  }
}

void suckStart() {
  state = SUCK;
  startTime = millis();
}

void suckStop() {
  int duration = millis() - startTime;
  if (duration < maxSipDuration) {
    if (previousAction == SIP) setPreviousAction(SIPSIP); 
    else setPreviousAction(SIP);
  }
  else {
    //lights out
    setColor(255,255,255);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// HELPER METHODS
//////////////////////////////////////////////////////////////////////////////////////////////

void sendEvent(int eventCode) {
  event = eventCode;
}

void setColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);  
}

void setPreviousAction(int action) {
  previousAction = action;
  actionStart = millis();
}

void toggleLED() {
  if (ledOn) digitalWrite(led, LOW);
  else digitalWrite(led, HIGH); 
  ledOn = !ledOn;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// FAKE IT UNTIL YOU MAKE IT
//////////////////////////////////////////////////////////////////////////////////////////////

void fake() {
  //this method sends a fake puff, a fake sip, a fake puffpuff, a fake sipsip, a long fake blow & a short fake suck
  
  basePressure = 10;
  maxPressure = 20;
  minPressure = 0; 
  
  // PUFF  
  if (millis()%20000 >= 1000 && millis()%20000 <= 1300) {
    currentPressure = maxPressure;
  }
  else if (millis()%20000 >= 1300 && millis()%20000 <= 4000) {
    currentPressure = basePressure;
  }
  
  // SIP
  else if (millis()%20000 >= 4000 && millis()%20000 <= 4300) {
    currentPressure = minPressure;
  }
  else if (millis()%20000 >= 4300 && millis()%20000 <= 7000) {
    currentPressure = basePressure;
  }
  
  // PUFFPUFF
  else if (millis()%20000 >= 7000 && millis()%20000 <= 7300) {
    currentPressure = maxPressure;
  }
  else if (millis()%20000 >= 7300 && millis()%20000 <= 7400) {
    currentPressure = basePressure;
  }
  else if (millis()%20000 >= 7400 && millis()%20000 <= 7700) {
    currentPressure = maxPressure;
  }
  else if (millis()%20000 >= 7700 && millis()%20000 <= 10000) {
    currentPressure = basePressure;
  }
  
  // SIPSIP
  else if (millis()%20000 >= 10000 && millis()%20000 <= 10300) {
    currentPressure = minPressure;
  }
  else if (millis()%20000 >= 10300 && millis()%20000 <= 10400) {
    currentPressure = basePressure;
  }
  else if (millis()%20000 >= 10400 && millis()%20000 <= 10700) {
    currentPressure = minPressure;
  }
  else if (millis()%20000 >= 10700 && millis()%20000 <= 13000) {
    currentPressure = basePressure;
  }
  
  // BLOW
  else if (millis()%20000 >= 13000 && millis()%20000 <= 14000) {
    currentPressure = 13;
  }
  else if (millis()%20000 >= 14000 && millis()%20000 <= 15000) {
    currentPressure = 17;
  }
  else if (millis()%20000 >= 15000 && millis()%20000 <= 16000) {
    currentPressure = 20;
  }
  else if (millis()%20000 >= 16000 && millis()%20000 <= 17000) {
    currentPressure = basePressure;
  }
  
  // SUCK
  else if (millis()%20000 >= 17000 && millis()%20000 <= 19000) {
    currentPressure = minPressure;
  }
  else {
    currentPressure = basePressure;
  }
}
