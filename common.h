/**
 *  @file           common.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          This file contains some common macro definitions for
 *                  configuring main relevants parameters
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/**
 *  @defgroup Common_defines_group Common Defines
 *   @{
 */

/**
 *  @def CAN_BAUDRATE
 *  @brief CAN network baud rate [\f$bps\f$]
 */
#define CAN_BAUDRATE        1000000

/**
 *  @def SERIAL_BAUDRATE
 *  @brief Serial UART baud rate [\f$bps\f$]
 */
#define SERIAL_BAUDRATE     115200

/**
 *  @def TIME_SLOT_PERIOD
 *  @brief Timer period [\f$ms\f$]
 */
#define TIME_SLOT_PERIOD	4000 // 4ms

/**
 *  @def TIMER
 *  @brief CANopen timer
 */
#define TIMER               Timer3

/**
 *  @defgroup SCU_firmware_selection SCU firmware selection
 *   @{
 *   The firmware for each node is selectable during the precompilation of the
 *   code from the directives present in that module.
 *   Comment/uncomment those macros for active/deactive selected SCU firmware.
 */

/**
 *  @def _FRONTAL_
 *  @brief Macro for selecting frontal SCU firmware
 */
#ifdef __DOXYGEN__
#define _FRONTAL_ // leave always uncommented
#endif
//#define _FRONTAL_ // comment/uncomment for active/deactive frontal SCU firmware

/**
 *  @def _RETRO_
 *  @brief Macro for selecting rear SCU firmware
 */
#define _RETRO_ // comment/uncomment for active/deactive read SCU firmware

#include "CAN_ID.h"

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* _COMMON_H_ */
