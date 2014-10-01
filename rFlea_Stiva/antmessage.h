/** \file antmessage.h
*
*  Contains ANT message IDs and other definiations useful for ANT messaging
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

#ifndef ANTMESSAGE_H
#define ANTMESSAGE_H

/////////////////////////////////////////////////////////////////////////////
// Message Format
// Messages are in the format:
//
// AX XX YY -------- CK
//
// where: AX    is the 1 byte sync byte either transmit or recieve
//        XX    is the 1 byte size of the message (0-249) NOTE: THIS WILL BE LIMITED BY THE EMBEDDED RECEIVE BUFFER SIZE
//        YY    is the 1 byte ID of the message (1-255, 0 is invalid)
//        ----- is the data of the message (0-249 bytes of data)
//        CK    is the 1 byte Checksum of the message
/////////////////////////////////////////////////////////////////////////////
#define MESG_TX_SYNC                      (0xA4)
#define MESG_RX_SYNC                      (0xA5)
#define MESG_SIZE_OFFSET                  (1)    
#define MESG_ID_OFFSET                    (2)     
#define MESG_SYNC_SIZE                    (1)
#define MESG_SIZE_SIZE                    (1)
#define MESG_ID_SIZE                      (1)
#define MESG_CHECKSUM_SIZE                (1)
#define MESG_MAX_DATA_SIZE                (9)     
#define MESG_HEADER_SIZE                  (MESG_SYNC_SIZE + MESG_SIZE_SIZE + MESG_ID_SIZE)
#define MESG_DATA_OFFSET                  MESG_HEADER_SIZE  
#define MESG_FRAME_SIZE                   (MESG_HEADER_SIZE + MESG_CHECKSUM_SIZE)
#define MESG_SAVED_FRAME_SIZE             (MESG_SIZE_SIZE + MESG_ID_SIZE)
#define MESG_MAX_SIZE                     (MESG_MAX_DATA_SIZE + MESG_FRAME_SIZE)
#define MESG_BUFFER_SIZE                  (MESG_MAX_DATA_SIZE + MESG_SAVED_FRAME_SIZE)

//////////////////////////////////////////////
// Buffer Indices
//////////////////////////////////////////////
#define BUFFER_INDEX_MESG_SIZE            (0x00)
#define BUFFER_INDEX_MESG_ID              (0x01)
#define BUFFER_INDEX_CHANNEL_NUM          (0x02)
#define BUFFER_INDEX_MESG_DATA            (0x03)
#define BUFFER_INDEX_RESPONSE_MESG_ID     (0x03)
#define BUFFER_INDEX_RESPONSE_CODE        (0x04)
#define BUFFER_INDEX_SHARED_ADDRESS_LSB   (0x03)
#define BUFFER_INDEX_SHARED_ADDRESS_MSB   (0x04)
#define BUFFER_INDEX_SHARED_DATA_TYPE     (0x05)

//////////////////////////////////////////////
// Message ID's
//////////////////////////////////////////////
#define MESG_INVALID_ID                   (0x00)
#define MESG_EVENT_ID                     (0x01)

#define MESG_APPVERSION_ID                (0x3D)  ///< application interface version
#define MESG_VERSION_ID                   (0x3E)  ///< protocol library version
#define MESG_RESPONSE_EVENT_ID            (0x40)

#define MESG_UNASSIGN_CHANNEL_ID          (0x41)
#define MESG_ASSIGN_CHANNEL_ID            (0x42)
#define MESG_CHANNEL_MESG_PERIOD_ID       (0x43)
#define MESG_CHANNEL_SEARCH_TIMEOUT_ID    (0x44)
#define MESG_CHANNEL_RADIO_FREQ_ID        (0x45)
#define MESG_NETWORK_KEY_ID               (0x46)
#define MESG_RADIO_TX_POWER_ID            (0x47)
#define MESG_RADIO_CW_MODE_ID             (0x48)
#define MESG_SEARCH_WAVEFORM_ID           (0x49)

#define MESG_SYSTEM_RESET_ID              (0x4A)
#define MESG_OPEN_CHANNEL_ID              (0x4B)
#define MESG_CLOSE_CHANNEL_ID             (0x4C)
#define MESG_REQUEST_ID                   (0x4D)

#define MESG_BROADCAST_DATA_ID            (0x4E)
#define MESG_ACKNOWLEDGED_DATA_ID         (0x4F)
#define MESG_BURST_DATA_ID                (0x50)

#define MESG_CHANNEL_ID_ID                (0x51)
#define MESG_CHANNEL_STATUS_ID            (0x52)
#define MESG_RADIO_CW_INIT_ID             (0x53)
#define MESG_CAPABILITIES_ID              (0x54)
#define MESG_SENSRCORE_DIGITAL_DATA       (0x40)

//////////////////////////////////////////////
// Message Sizes
//////////////////////////////////////////////
#define MESG_INVALID_SIZE                 (0)

#define MESG_RESPONSE_EVENT_SIZE          (3)
#define MESG_CHANNEL_STATUS_SIZE          (2)
#define MESG_VERSION_SIZE                 (9)

#define MESG_UNASSIGN_CHANNEL_SIZE        (1)
#define MESG_ASSIGN_CHANNEL_SIZE          (3)
#define MESG_CHANNEL_ID_SIZE              (5)
#define MESG_CHANNEL_MESG_PERIOD_SIZE     (3)
#define MESG_CHANNEL_SEARCH_TIMEOUT_SIZE  (2)
#define MESG_CHANNEL_RADIO_FREQ_SIZE      (2)
#define MESG_NETWORK_KEY_SIZE             (9)
#define MESG_RADIO_TX_POWER_SIZE          (2)
#define MESG_RADIO_CW_MODE_SIZE           (3)
#define MESG_RADIO_CW_INIT_SIZE           (1)
#define MESG_SEARCH_WAVEFORM_SIZE         (3)

#define MESG_SYSTEM_RESET_SIZE            (1)
#define MESG_OPEN_CHANNEL_SIZE            (1)
#define MESG_CLOSE_CHANNEL_SIZE           (1)
#define MESG_REQUEST_SIZE                 (2)
#define MESG_CAPABILITIES_SIZE            (4)

#define MESG_DATA_SIZE                    (9)

#endif // !ANTMESSAGE_H
