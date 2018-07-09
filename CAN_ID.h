/** 
 *  @file           CAN_ID.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CAN servizi nodeIDs module header
 */

#ifndef _CAN_ID_H_
#define _CAN_ID_H_

#include "common.h"

/**
 *  @defgroup CAN_network_module CAN Servizi Network
 *   @{
 */

/**
 *      @defgroup CAN_ID CAN Network Nodes ID
 *       @{
 */

/**
 *  @def SCU_FRONTAL_NODE_ID
 *  @brief Frontal SCU node ID on CAN servizi network
 */
#define SCU_FRONTAL_NODE_ID     1

/**
 *  @def VCU_NODE_ID
 *  @brief VCU node ID on CAN servizi network
 */
#define VCU_NODE_ID             2

/**
 *  @def SCU_REAR_NODE_ID
 *  @brief Rear SCU node ID on CAN servizi network
 */
#define SCU_REAR_NODE_ID	    3

/**
 *  @def TCU_NODE_ID
 *  @brief TCU node ID on CAN servizi network
 */
#define TCU_NODE_ID             4

/**
 *       @}
 */

/**
 *  @}
 */

#endif /* _CAN_ID_H_ */