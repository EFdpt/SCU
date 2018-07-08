/** 
 *  @file           nmt.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANOpen NMT module header
 */

#ifndef _NMT_H_
#define _NMT_H_

#include "CO_can.h"

/**
 *  @addtogroup CAN_network_module
 *   @{
 */

/** @defgroup CANopen_NMT_group CANopen NMT module
 *   @{
 */

/**
 *  @brief      This function manages an NMT request from master node on CAN servizi network.
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in] m    Received NMT message
 */
void proceedNMTstateChange(Message* m);

/**
 *  @brief      This function sends a slave boot-up message over CAN servizi network.
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void slaveSendBootUp();

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* _NMT_H_ */