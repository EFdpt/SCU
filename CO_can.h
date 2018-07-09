/** 
 *  @file           CO_can.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANOpen main module header
 */

#ifndef _CO_CAN_H
#define _CO_CAN_H

#include "CAN_ID.h"
#include <Arduino.h>

/**
 *  @page CAN_network_page CAN Servizi network
 *  
 *  Two CAN networks have been designed to be inserted into the vehicle: a first
 *  CAN network between the VCU and the inverter (CAN funzionale) and a second 
 *  CAN network between the VCU, TCU and SCUs (CAN servizi).
 *
 *  Each node connected to CAN servizi network has an unique ID into that network, 
 *  according to this table:<br/><br/>
 *  | @anchor NODE-ID NODE | NODE-ID |
 *  | :------------------: | :-----: |
 *  | \f$SCU_{FRONTAL}\f$  | 1       |
 *  | \f$VCU\f$            | 2       |
 *  | \f$SCU_{REAR}\f$     | 3       |
 *  | \f$TCU\f$            | 4       |
 *  <br/>
 * 
 * -----------------------------------------------------------
 *     
 *  <h2>SCU slave on power up sequence</h2>
 *  1. **send BOOT-UP message after initialisation state**<br/><br/>
 *      | COB-ID (11bits) | data byte 0 |
 *      | :-------------: | :---------: |
 *      | 0x700 + NODE-ID | 0x00        |
 *      <br/>
 * 
 * -----------------------------------------------------------
 *      
 *  2. **wait NMT 'go Operational' from VCU master node**<br/><br/>
 *      | COB-ID (11bits) | data byte 0 | data byte 1 |
 *      | :-------------: | :---------: | :---------: |
 *      | 0x000           | 0x01        | 0x00        |
 *      <br/>
 * 
 * -----------------------------------------------------------
 *
 *  3. **@anchor TPDO_Timer periodically send TPDOs with sensors' data**
 *      Each node starts a timer with #TIME_SLOT_PERIOD period.
 *      In this way one slot (or more) of #TIME_SLOT_PERIOD is assigned to each
 *      node for transmission, so as to reduce CAN bus load.<br/><br/>
 *      | START TIMER           | packet #1           | packet #2           | packet #3        | packet #4 |
 *      | :-------------------: | :-----------------: | :-----------------: | :--------------: | :-------: |
 *      | \f$VCU\f$             | \f$SCU_{FRONTAL}\f$ | \f$SCU_{FRONTAL}\f$ | \f$SCU_{REAR}\f$ | \f$TCU\f$ | 
 *      |                       | TRANSMISSION PERIOD                                                   ||||
 *      <br/>
 *
 * -----------------------------------------------------------
 * 
 *  <h2>@anchor TPDO_configuration TPDO configuration</h2>
 *      | TX NODE             | Data               | Unit          | Data Length    | Data Offset   | #CAN packet | ID              | Total Length |
 *      | :-----------------: | :----------------: | :-----------: | :------------: | :-----------: | :---------: | :-------------: | :----------: |
 *      | \f$SCU_{FRONT}\f$   | First APPS         | \f$\%\f$      | \f$1\ Byte\f$  | \f$[0:7]\f$   | #1          | TPDO1 + NODE-ID | 4            |
 *      | ^                   | Second APPS        | \f$\%\f$      | \f$1\ Byte\f$  | \f$[8:15]\f$  | ^           | ^               | ^            |
 *      | ^                   | Brake              | \f$\%\f$      | \f$1\ Byte\f$  | \f$[16:23]\f$ | ^           | ^               | ^            |
 *      | ^                   | APPS plausibility  | \f$bool\f$    | \f$4\ bit\f$   | \f$[24:27]\f$ | ^           | ^               | ^            |
 *      | ^                   | Brake plausibility | \f$bool\f$    | \f$4\ bit\f$   | \f$[28:31]\f$ | ^           | ^               | ^            |
 *      | ^                   | Right phonic wheel | \f$rpm\f$     | \f$2\ Bytes\f$ | \f$[0:15]\f$  | #2          | TPDO2 + NODE-ID | 6            |
 *      | ^                   | Left phonic wheel  | \f$rpm\f$     | \f$2\ Bytes\f$ | \f$[16:31]\f$ | ^           | ^               | ^            |
 *      | ^                   | Right suspension   | \f$mm\f$      | \f$1\ Byte\f$  | \f$[32:39]\f$ | ^           | ^               | ^            |
 *      | ^                   | Left suspension    | \f$mm\f$      | \f$1\ Byte\f$  | \f$[40:47]\f$ | ^           | ^               | ^            |
 *      | \f$SCU_{REAR}\f$    | Accel. X           | \f$m/s^{2}\f$ | \f$1\ Byte\f$  | \f$[0:7]\f$   | #3          | TPDO1 + NODE-ID | 8            |
 *      | ^                   | Accel. Z           | \f$m/s^{2}\f$ | \f$1\ Byte\f$  | \f$[8:15]\f$  | ^           | ^               | ^            |
 *      | ^                   | Right suspension   | \f$mm\f$      | \f$1\ Byte\f$  | \f$[16:23]\f$ | ^           | ^               | ^            |
 *      | ^                   | Left suspension    | \f$mm\f$      | \f$1\ Byte\f$  | \f$[24:31]\f$ | ^           | ^               | ^            |
 *      | ^                   | Right phonic wheel | \f$rpm\f$     | \f$2\ Bytes\f$ | \f$[32:47]\f$ | ^           | ^               | ^            |
 *      | ^                   | Left phonic wheel  | \f$rpm\f$     | \f$2\ Bytes\f$ | \f$[48:63]\f$ | ^           | ^               | ^            |
 *      | \f$TCU\f$           | Torque limiter     | \f$\%\f$      | \f$1\ Byte\f$  | \f$[0:7]\f$   | #4          | TPDO1 + NODE-ID | 1            |
 *      *where TPDO1 = 0x180 and TPDO2 = 0x280*
 */

/**
 *  @addtogroup CAN_network_module
 *   @{
 */

/// CANopen message struct
typedef struct {
    uint16_t cob_id;    /*!< message's COB-ID */
    uint8_t len;        /*!< message's length (0 to 8) */
    uint8_t data[8];    /*!< message's datas */
} Message;

/**
 *  @def Message_Initializer
 *  @brief CANopen static message initializer
 */
#define Message_Initializer {0,0,{0,0,0,0,0,0,0,0}}

/**
 *  @brief      This function initialize CAN/CANopen interfaces and communication.
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void initCAN();

/**
 *  @brief      This function send a CANopen message over CAN servizi network.
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in] m    CANopen message to send
 */
void canSend(Message* m);

/**
 *  @brief      This function returns node ID into CAN servizi network
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @return     CANopen node ID
 */
uint8_t getNodeId();

/**
 *  @brief      This function sets node ID into CAN servizi network
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in]  nodeId   CANopen node ID
 */
void setNodeId(uint8_t nodeId);

/**
 *  @}
 */

#endif /* _CO_CAN_H_ */