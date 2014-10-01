#include "Arduino.h"
#include "ANT.h"
#include "antmessage.h"
#include "antdefines.h"
#include <EEPROM.h>


ANT::ANT()
{
  msgInPos = 0;
  msgSize = 0;
  msgStarted = false;
  msgTxPos = 0;
  msgTxSize = 0;
  pinMode(AntSleep,OUTPUT);
  pinMode(AntSuspend,OUTPUT);
  pinMode(AntReset,OUTPUT);
  pinMode(AntRTS,INPUT);
  pinMode(RxInterrupt,INPUT);
  }

void ANT::initANT(){
  sleep(false);
  digitalWrite(AntSuspend,HIGH);
  HWreset();
  sleep(true);
  
  //Instead of this delay, we have to wait until startup message from ant, if not received, ANT not present
  //Improvement: add waiting and notify if it went well. Remove delay after fixing that
  delay(500);
}
void   ANT::deepSleep(){
sleep(false);
 uint8_t msgbuf[2];
  msgbuf[0] = 0xc5; // ID Byte
  msgbuf[1] = 0x00; 
  txMessage(msgbuf, 2);
  flushTx();
sleep(true);
}

void ANT::sleep(bool status){
  digitalWrite(AntSleep,status);
}


//IMPROVEMENT: add ANTdefinitions and use. Thats all
void ANT::txMessage(uint8_t* message, uint8_t  messageSize)
{
  byte i;
  txBufferPos   = 0;                       // set position to 0
  txBufferSize  = messageSize + 3;         // message plus sync, size and checksum
  txBuffer[0]   = MESG_TX_SYNC;                    // sync byte
  txBuffer[1]   = (uint8_t) messageSize - 1; // message size - command size (1)

  for(i=0; i<messageSize; i++)
    txBuffer[2+i] = message[i];

  // calculate the checksum
  txBuffer[txBufferSize - 1]=0;
  for(i=0; i<txBufferSize - 1; ++i){
    txBuffer[txBufferSize - 1] = txBuffer[txBufferSize - 1] ^ txBuffer[i];
  }

  for(i=0; i<txBufferSize; i++)
  {
    msgTxBuf[msgTxPos++] = txBuffer[i]; 
    msgTxSize++;
  }
}


void ANT::flushTx(){
  sleep(false);
  while (digitalRead(AntRTS)){
	// If RTS is hight, don't send and wait for it to go low
	}
  for(int i=0; i<msgTxSize; i++)
  {
    Serial.write(msgTxBuf[i]); 
  }
  msgTxSize =0;
  msgTxPos = 0;
  
  //WAIT for RTS to notify about the received message
  while (!digitalRead(AntRTS)){
	// If RTS goes high (50us) it means message recieved and correct. If not received, something went wrong
	// until watchdog implemented this code could hang here in case a message is corrupted and RTS never replied
	// IMPROVEMENT HERE: add a timer (watchdog) to leave in case of corrupted message
	}
  sleep(true);
  update();
}

// Resets module
//IMPRVOEMENTS: use ANT definitions 
void ANT::SWreset()
{
  uint8_t msgbuf[2];
  msgbuf[0] = 0x4a; // ID Byte
  msgbuf[1] = 0x00; 
  txMessage(msgbuf, 2);
  flushTx();
}

void ANT::HWreset()
{
  digitalWrite(AntReset,LOW);
  delay(1);
  digitalWrite(AntReset,HIGH);
}

//IMPRVOEMENTS: use ANT definitions 
void ANT::closech(antConfiguration configuration)
{
  uint8_t msgbuf[2];
  msgbuf[0] = 0x4c; 
  msgbuf[1] = configuration.channelNR; 
  txMessage(msgbuf, 2);
  flushTx();
}


//IMPRVOEMENTS: use ANT definitions 
// Assigns CH=0, CH Type=10(TX), Net#=0
void ANT::assignch(antConfiguration configuration)
{
  uint8_t msgbuf[4];
  msgbuf[0] = 0x42;
  msgbuf[1] = configuration.channelNR;    // Channel ID, 0x00 for HRM, 0x01 for custom
  msgbuf[2] = configuration.channelType;  // CH Type
  msgbuf[3] = configuration.networkID;   // Network ID
  txMessage(msgbuf, 4);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions 
void ANT::assignchExt(antConfiguration configuration, byte Ext)
{
  uint8_t msgbuf[5];
  msgbuf[0] = 0x42;
  msgbuf[1] = configuration.channelNR;    // Channel ID, 0x00 for HRM, 0x01 for custom
  msgbuf[2] = configuration.channelType;  // CH Type
  msgbuf[3] = configuration.networkID;   // Network ID
  msgbuf[4] = Ext;
  txMessage(msgbuf, 5);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
// Assigns CH#, Device#=0000, Device Type ID=00, Trans Type=00
void ANT::setchid(antConfiguration configuration)
{
  uint8_t msgbuf[6];
  msgbuf[0] = 0x51;
  msgbuf[1] = configuration.channelNR;      // Channel Number, 0x00 for HRM
   msgbuf[2] = (configuration.deviceID & 0xFF00)>>8;    // Device Number MSB
  msgbuf[3] = (configuration.deviceID & 0xFF);    // Device Number LSB
 msgbuf[4] = configuration.deviceType;   // Device Type, 0x78 for HRM
  msgbuf[5] = configuration.txType;
  txMessage(msgbuf, 6);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::setrf(antConfiguration configuration)
{
  uint8_t msgbuf[3];
  msgbuf[0] = 0x45;
  msgbuf[1] = configuration.channelNR;      // Channel Number, 0x00 for HRM
  msgbuf[2] = configuration.channelFreq;         // lower byte for RF freq
  txMessage(msgbuf, 3);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::setchperiod(antConfiguration configuration)
{
  uint8_t msgbuf[4];
  msgbuf[0] = 0x43;
  msgbuf[1] = configuration.channelNR;      // Channel Number, 0x00 for HRM
  msgbuf[2] = (configuration.channelPeriod & 0xFF);        // Ch period MSB
  msgbuf[3] = (configuration.channelPeriod & 0xFF00) >> 8; // Ch period LSB
  txMessage(msgbuf, 4);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::setSearchTimeout(antConfiguration configuration, byte to){
  uint8_t msgbuf[3];
  msgbuf[0] = 0x44; 
  msgbuf[1] = configuration.channelNR; 
  msgbuf[2] = to; 
  txMessage(msgbuf, 3);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::setLowPrioritySearchTimeout(antConfiguration configuration, byte to){
  uint8_t msgbuf[3];
  msgbuf[0] = 0x63; 
  msgbuf[1] = configuration.channelNR; 
  msgbuf[2] = to; 
  txMessage(msgbuf, 3);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
// Opens CH 
void ANT::opench(antConfiguration configuration)
{
  uint8_t msgbuf[2];
  msgbuf[0] = 0x4b;
  msgbuf[1] = configuration.channelNR;
  txMessage(msgbuf, 2);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::enableExtClock(){
  uint8_t msgbuf[2];
  msgbuf[0] = 0x6D; 
  msgbuf[1] = 0x00; 
  txMessage(msgbuf, 2);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::setTxPower(antConfiguration configuration,byte power){ //0 = -18dBm, 1 = -12dBm, 2 = -6dBm, 3 = 0dBm = default
 uint8_t msgbuf[3];
  msgbuf[0] = 0x60; 
  msgbuf[1] = configuration.channelNR; 
  msgbuf[2] = power; 
  txMessage(msgbuf, 3);
  flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::setProximitySearch(antConfiguration configuration,byte radius){ //0 – disabled;  1:10 – closest to farthest
  uint8_t msgbuf[3];
  msgbuf[0] = 0x71; 
  msgbuf[1] = configuration.channelNR; 
  msgbuf[2] = radius; 
  txMessage(msgbuf, 3);
  flushTx();
}


//IMPRVOEMENTS: use ANT definitions
void ANT::contScan(){
  uint8_t msgbuf[2];
  msgbuf[0] = 0x5B; 
  msgbuf[1] = 0x00; 
  txMessage(msgbuf, 2);
   flushTx();
}

//IMPRVOEMENTS: use ANT definitions
void ANT::libConfig(byte co){
  uint8_t msgbuf[3];
  msgbuf[0] = 0x6e; 
  msgbuf[1] = 0x00;
  msgbuf[2] = co; 
  txMessage(msgbuf, 3);
  flushTx();
}

void ANT::config(antConfiguration conf,int msec)
{
  assignch(conf);    
  delay(msec);
  
  setchid(conf);     
  delay(msec);

  setrf(conf);       
  delay(msec);
 
  setchperiod(conf); 
  delay(msec);
} 
  
  void ANT::sendData(antConfiguration  conf, byte* message){
    uint8_t msgbuf[10];
    msgbuf[0] = 0x4e;
    msgbuf[1] = conf.channelNR; //channel
	for(int i =2;i<10;i++){
		msgbuf[i] = message[i-2];
	}
    txMessage(msgbuf, 10);
    flushTx();  
  }
  
    void ANT::sendTxData(byte* message, byte messageSize){
    txMessage(message, 10);
    flushTx();  
  }
  
  void ANT::sendAcknowledgedData(antConfiguration  conf, byte* message, byte messageSize){
    uint8_t msgbuf[10];
    msgbuf[0] = 0x4f;
    msgbuf[1] = conf.channelNR; //channel
	for(int i =2;i<10;i++){
		msgbuf[i] = message[i-2];
	}
    txMessage(msgbuf, 10);
    flushTx();  
  }

byte* ANT::update(){
    while (Serial.available()){
      byte t = Serial.read();
      if(msgInPos>30){
        msgStarted = false;
        msgInPos=0;
        msgSize = 0;
      }
      if (!msgStarted && t == MESG_TX_SYNC){
        msgStarted = true;
      }
      if(msgStarted){  
        msgInBuf[msgInPos] = t;
        if (msgInPos == MESG_SIZE_OFFSET){
          msgSize = t + MESG_FRAME_SIZE;
        }
        msgInPos++;
        if(msgInPos == msgSize){	
          // calculate the checksum
          byte checkSum = 0;
          for(int i=0; i<msgSize - 1; i++){
            checkSum = checkSum ^ msgInBuf[i];
          } 

          if(checkSum == msgInBuf[msgSize-1]){ 	
			msgStarted = 0; 
          	msgInPos=0;
          	msgSize = 0; 
            return(msgInBuf);
          }
          else{
            msgStarted = 0; 
          	msgInPos=0;
          	msgSize = 0; 
            return(NULL);
			}
          }
        }
    }
  if(msgTxSize>0){
    flushTx();
  }	
  return (NULL);
}







