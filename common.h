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

#define CAN_PORT            Can0
#define CAN_BAUDRATE        1000000 // 1Mb/s

#define SERIAL_BAUDRATE     115200

#define TIME_SLOT_PERIOD	4000 // 4ms

//#define _FRONTAL_
#define _RETRO_

#include "CAN_ID.h"

#endif /* _COMMON_H_ */
