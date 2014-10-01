#ifndef _ANT_h
#define _ANT_h

#include <inttypes.h>
#include <EEPROM.h>



#define AntReset  6
#define AntSleep  8
#define AntSuspend  9
#define AntRTS 7
#define RxInterrupt 2
#define TxBufferSize 64


typedef uint8_t byte;

struct antConfiguration {
  byte channelNR;
  byte channelType;
  byte networkID;
  int deviceID;
  byte deviceType;
  byte txType;
  byte channelFreq;
  int channelPeriod;
  byte channelStatus;
};



class ANT 
{
public:
  ANT();
  void SWreset(void);
  void HWreset();
  void deepSleep();
  void initANT(); 
  void closech(antConfiguration conf);
  void assignch(antConfiguration  conf); 
  void assignchExt(antConfiguration configuration, byte Ext);
  void setrf(antConfiguration  conf); 
  void setchperiod(antConfiguration  conf);
  void setchid(antConfiguration  conf);  
  void opench(antConfiguration conf);   
  void config(antConfiguration conf,int msec);  
  void setTxPower(antConfiguration configuration,byte power);
  void setProximitySearch(antConfiguration configuration,byte radius);
  void checkSerial();
  void checkSerialOld();
  byte* update();
  void sendData(antConfiguration  conf,byte Addr);
  void sendData(antConfiguration  conf,int load);
  void sendData(antConfiguration  conf, byte* message);
  void sendData(byte _channel, byte* message, byte messageSize);
  void sendAcknowledgedData(antConfiguration  conf,byte Addr);
  void sendAcknowledgedData(antConfiguration  conf,int load);
  void sendAcknowledgedData(antConfiguration  conf, byte* message, byte messageSize);
  void sendAcknowledgedData(byte _channel, byte* message, byte messageSize);
  void sendTxData(byte* message, byte messageSize);
  void setSearchTimeout(antConfiguration conf, byte to);
  void setLowPrioritySearchTimeout(antConfiguration conf, byte to);
  void libConfig(byte co);
  void contScan();
  void enableExtClock();  
  void sleep(bool status);
  
private:
  int SerialNumber;
  
  byte txBuffer[32];
  byte txBufferSize;
  byte txBufferPos;
  
  byte msgInBuf[32];
  byte msgInPos;
  byte msgSize;
  bool msgStarted;
  
  byte msgTxBuf[64];
  byte msgTxPos;
  byte msgTxSize;
   
  void flushTx();
  void parseData(byte* message, byte messageSize);
  void txMessage(uint8_t* message, uint8_t  messageSize);
  void processData(); 
};

#endif