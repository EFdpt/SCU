/** 
 *  @file           pdo.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANopen PDO support header file
 */

#ifndef _PDO_H_
#define _PDO_H_

#include "def.h"
#include "CO_can.h"
#include "common.h"

/**
 *  @addtogroup CAN_network_module
 *   @{
 */

/**
 *  @defgroup CANopen_PDO_module CANopen PDO module
 *   @{
 *   Data into PDOs are configured according @ref TPDO_configuration.
 */

/**
 *  @brief      This function serializes data to send into PDO message.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in]  PDOtype     PDO type according to @ref CANopen_Func_Codes.
 *  @param[in]  pdo         CANopen message to build
 */
void buildPDO(uint8_t PDOtype, Message* pdo);

#if defined(_RETRO_)
/**
 *  @brief      This function manages PDO message receive, deserializing data.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in]  pdo     CANopen message to manage
 */
void proceedPDO(Message* pdo);
#endif

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* _PDO_H_ */