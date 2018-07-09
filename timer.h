/** 
 *  @file           timer.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANopen timer header file
 */

#ifndef _TIMER_H_
#define _TIMER_H_

/**
 *  @addtogroup CAN_network_module
 *   @{
 */

/**
 *  @defgroup CANopen_timer_module CANopen Timer module
 *   @{
 */

/**
 *  @brief      Initialize timer for periodic submit of TPDOs.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void timerInit();

/**
 *  @brief      Start timer for periodic submit of TPDOs.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void timerStart();

/**
 *  @brief      Stop timer for periodic submit of TPDOs.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void timerStop();

/**
 *  @brief      Dispatch time slot for each CANopen node according to @ref TPDO_Timer.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void TimeDispatch();

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* _TIMER_H_ */