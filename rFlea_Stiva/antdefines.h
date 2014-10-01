/** \file antdefines.h
*
* Conatins definitions for ANT config parameters and ANT event codes
*
* $Name:  $
*/
/*
 * Copyright (C) 2006 Dynastream Innovations Inc. All rights reserved. 
 *
 * The intellectual property contained within this file is sole property of Dynastream Innovations Inc. 
 * Any distribution without the prior written consent of Dynastream Innovations Inc. is strictly prohibited.
 *
 * Dynastream Innovations Inc. does not guarantee error free operation of the intellectual property contained 
 * herein. 
 * 
 * Please forward all questions regarding this code to ANT Technical Support.
 * 
 * Dynastream Innovations Inc.
 * 228 River Avenue
 * Cochrane, Alberta, Canada
 * T4C 2C1
 * 
 * (P) (403) 932-9292
 * (F) (403) 932-6521
 * (TF) 1-866-932-9292 in North America
 * (E) support@thisisant.com
 * 
 * www.thisisant.com
 */ 

#ifndef ANTDEFINES_H
#define ANTDEFINES_H

//////////////////////////////////////////////
// ANT Clock Definition
//////////////////////////////////////////////
#define ANT_CLOCK_FREQUENCY                        ((ULONG)32768)          ///< ANT system clock frequency

//////////////////////////////////////////////
// Radio TX Power Definitions
//////////////////////////////////////////////
#define RADIO_TX_POWER_MASK                        (0x03)
#define RADIO_TX_POWER_MINUS20DB                   (0x00)
#define RADIO_TX_POWER_MINUS10DB                   (0x01)
#define RADIO_TX_POWER_MINUS5DB                    (0x02)
#define RADIO_TX_POWER_0DB                         (0x03)

//////////////////////////////////////////////
// Default System Definitions
//////////////////////////////////////////////
#define DEFAULT_CHANNEL_MESSAGE_FREQUENCY          ((ULONG)4)
#define DEFAULT_CHANNEL_MESSAGE_PERIOD             ((USHORT)( ANT_CLOCK_FREQUENCY / DEFAULT_CHANNEL_MESSAGE_FREQUENCY )) ///< 8192 (4Hz)
#define DEFAULT_RADIO_TX_POWER                     RADIO_TX_POWER_0DB      ///< ANT default RF power
#define DEFAULT_RADIO_CHANNEL                      (66)             ///< 2400MHz + 1MHz * Channel Number = 2466MHz
#define DEFAULT_RX_SEARCH_TIMEOUT                  (12)             ///< 12 * 2.5 seconds = 30 seconds

//////////////////////////////////////////////
// ID Definitions
//////////////////////////////////////////////
#define ID_MANUFACTURER_OFFSET                     (3)
#define ID_DEVICE_TYPE_OFFSET                      (2)
#define ID_DEVICE_NUMBER_HIGH_OFFSET               (1)
#define ID_DEVICE_NUMBER_LOW_OFFSET                (0)
#define ID_DEVICE_TYPE_PAIRING_FLAG                (0x80)

//////////////////////////////////////////////
// Assign Channel Parameters
//////////////////////////////////////////////
#define PARAMETER_TX_NOT_RX                        (0x10)
#define PARAMETER_MULTIPLE_ACCESS_CHANNEL          (0x20)  
#define PARAMETER_NO_TX_GUARD_BAND                 (0x40)   
#define PARAMETER_ALWAYS_RX_WILD_CARD_SEARCH_ID    (0x40)

//////////////////////////////////////////////
// Assign Channel Types
//////////////////////////////////////////////
#define CHANNEL_TYPE_SLAVE                         ( 0x00)
#define CHANNEL_TYPE_MASTER                        ( 0x10)
#define CHANNEL_TYPE_MASTER_TX_ONLY                ( 0x50)
#define CHANNEL_TYPE_SHARED_SLAVE                  ( 0x20)
#define CHANNEL_TYPE_SHARED_MASTER                 ( 0x30)

//////////////////////////////////////////////
// Channel Status
//////////////////////////////////////////////
#define STATUS_UNASSIGNED_CHANNEL                  (0x00)
#define STATUS_ASSIGNED_CHANNEL                    (0x01)
#define STATUS_SEARCHING_CHANNEL                   (0x02)
#define STATUS_TRACKING_CHANNEL                    (0x03)
#define STATUS_OVERRUN                             (0x40)
#define STATUS_UNDERRUN                            (0x80)

//////////////////////////////////////////////
// Standard capabilities defines
//////////////////////////////////////////////
#define CAPABILITIES_NO_RX_CHANNELS                (0x01)
#define CAPABILITIES_NO_TX_CHANNELS                (0x02)
#define CAPABILITIES_NO_RX_MESSAGES                (0x04)
#define CAPABILITIES_NO_TX_MESSAGES                (0x08)
#define CAPABILITIES_NO_ACKD_MESSAGES              (0x10)
#define CAPABILITIES_NO_BURST_TRANSFER             (0x20)

//////////////////////////////////////////////
// Advanced capabilities defines
//////////////////////////////////////////////
#define CAPABILITIES_DETECT_OVERRUN_UNDERRUN       (0x01)
#define CAPABILITIES_NETWORK_ENABLED               (0x02)

//////////////////////////////////////////////
// Burst Message Sequence 
//////////////////////////////////////////////
#define CHANNEL_NUMBER_MASK                        (0x1F)
#define SEQUENCE_NUMBER_MASK                       (0xE0)
#define SEQUENCE_NUMBER_INC                        (0x20)
#define SEQUENCE_NUMBER_ROLLOVER                   (0x60)
#define SEQUENCE_LAST_MESSAGE                      (0x80)

//////////////////////////////////////////////
// Shared Channel Commands / Datatypes
//////////////////////////////////////////////
#define SHARED_CMD_SLOT_AVALIBLE                   (0xFF)
#define SHARED_CMD_BUSY_ACQUIRING                  (0xFE)
#define SHARED_CMD_COMMAND_REQUEST_TO_ACQUIRE      (0xFD)
#define SHARED_CMD_CONFIRM_ACQUIRED                (0xFC)
#define SHARED_CMD_NO_SLOTS_AVAILABLE              (0xFB)
//...
#define SHARED_TYPE_RELAY                          (0x43)           
#define SHARED_TYPE_COUNTER                        (0x42)
#define SHARED_TYPE_A_TO_D                         (0x41)
#define SHARED_TYPE_DIGITAL                        (0x40)           
#define SHARED_TYPE_UNDEFINED                      (0x00)

///////////////////////////////////////////////////////////////////////
// AtoD SubTypes
///////////////////////////////////////////////////////////////////////
#define TEMPERATURE                                (0xFE)
#define BATT_VOLTAGE                               (0xFF)

//////////////////////////////////////////////
// Response / Event Codes
//////////////////////////////////////////////
#define RESPONSE_NO_ERROR                          (0x00)             

#define EVENT_RX_SEARCH_TIMEOUT                    (0x01)             
#define EVENT_RX_FAIL                              (0x02)             
#define EVENT_TX                                   (0x03)             
#define EVENT_TRANSFER_RX_FAILED                   (0x04)             
#define EVENT_TRANSFER_TX_COMPLETED                (0x05)             
#define EVENT_TRANSFER_TX_FAILED                   (0x06)             
#define EVENT_CHANNEL_CLOSED                       (0x07)
#define EVENT_RX_FAIL_GO_TO_SEARCH                 (0x08)

#define CHANNEL_IN_WRONG_STATE                     (0x15)           ///< returned on attempt to perform an action from the wrong channel state
#define CHANNEL_NOT_OPENED                         (0x16)           ///< returned on attempt to communicate on a channel that is not open
#define CHANNEL_ID_NOT_SET                         (0x18)           ///< returned on attempt to open a channel without setting the channel ID

#define TRANSFER_IN_PROGRESS                       (0x1F)           ///< returned on attempt to communicate on a channel with a TX transfer in progress
#define TRANSFER_SEQUENCE_NUMBER_ERROR             (0x20)           ///< returned when sequence number is out of order on a Burst transfer

#define INVALID_MESSAGE                            (0x28)           ///< returned when the message has an invalid parameter
#define INVALID_NETWORK_NUMBER                     (0x29)           ///< returned when an invalid network number is provided

#endif // !ANTDEFINES_H                                                    


