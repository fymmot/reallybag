#include "Arduino.h"
#include "rFlea_Stiva.h"
#include "antmessage.h"
#include "Arduino.h"
#include "antdefines.h"
#include "antmessage.h"
#include "rFlea_Stiva.h"
#include <ANT.h>
#include <EEPROM.h>


rFlea_Stiva::rFlea_Stiva()
{  
  CH0.channelNR=0;
  CH0.channelType=PARAMETER_TX_NOT_RX;
  CH0.networkID=0x00;
  CH0.deviceID=getSerialNumber();
  CH0.deviceType=0x27;
  CH0.txType=0x01;
  CH0.channelFreq=DEFAULT_RADIO_CHANNEL;
  CH0.channelPeriod=2048;
  CH0.channelStatus=STATUS_UNASSIGNED_CHANNEL;
}

void rFlea_Stiva::init(){
  SerialNumber=CH0.deviceID;
  ant.initANT();
  ant.enableExtClock(); //low power
  ant.config(CH0,200); //configure channel 0
}

void rFlea_Stiva::connect(){
  ant.opench(CH0);
}

void rFlea_Stiva::disconnect(){
  ant.closech(CH0);
}

void rFlea_Stiva::update(){
  parseData(ant.update());
}

void rFlea_Stiva::parseData(byte* message){
  switch (message[MESG_ID_OFFSET]){
  case MESG_BROADCAST_DATA_ID:
    {
		onMessageCallback(message);
      break;
    }
  case MESG_ACKNOWLEDGED_DATA_ID:
    {

      break;
    }
  case MESG_RESPONSE_EVENT_ID:
    {
    //Channel Response / Event
      switch (message[5]){
      case RESPONSE_NO_ERROR: // EVENT_TX
        { 

          break; 
        }
        case EVENT_RX_SEARCH_TIMEOUT: // EVENT_TX
        { 




          break; 
        }
        case EVENT_RX_FAIL: // EVENT_TX
        { 




          break; 
        }
      case EVENT_TX: // EVENT_TX
        {
			onSyncCallback();
          break; 
        }  
	  case EVENT_TRANSFER_RX_FAILED: // EVENT_TX
        { 





          break; 
        }
              case EVENT_TRANSFER_TX_COMPLETED: // EVENT_TX
        { 





          break; 
        }
              case EVENT_TRANSFER_TX_FAILED: // EVENT_TX
        { 





          break; 
        }
              case EVENT_CHANNEL_CLOSED: // EVENT_TX
        { 






          break; 
        }
              case EVENT_RX_FAIL_GO_TO_SEARCH: // EVENT_TX
        { 





          break; 
        }    
      default:
        {
            ///channelEventResponseCallback(message,messageSize);
        }

      }

      break;
    }
    default:{
            //////unknownResponseCallback(message,messageSize);    
    }		
  }	
}

void rFlea_Stiva::register_onMessage(void (*callback_funct)(byte * message)){
  onMessageCallback=callback_funct;
}
void rFlea_Stiva::register_onSync(void (*callback_funct)()){
  onSyncCallback=callback_funct;
}
  
void rFlea_Stiva::send(byte* message){
	ant.sendData(CH0, message);
}

int rFlea_Stiva::getSerialNumber(){

 SerialNumber =  ((EEPROM.read(0x00)<<8) & 0xFF00) + EEPROM.read(0x01); 
  if(SerialNumber == 0xFFFF){ //no SerialNumber 

    Serial.println("No Serial found .. flashing new one:");   
    randomSeed(analogRead(A2));

    EEPROM.write(0, random(255)); //Serial Number MSB
    EEPROM.write(1, random(255)); //Serial Number LSB
    EEPROM.write(2, 0x54); //Board Type ... 54 = Stiva
    EEPROM.write(3, 0x01); //Version Number  01 = 0.1 
  }

  SerialNumber =  ((EEPROM.read(0x00)<<8) & 0xFF00) + EEPROM.read(0x01); 


return SerialNumber;
}