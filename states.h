/** 
 *  @file           states.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANopen finite state machine header file
 */

#ifndef _STATES_H_
#define _STATES_H_

#include "CO_can.h"

/**
 *  @page FSM_page Finite State Machine (FSM)
 *  
 *  Each SCU board can be represented by a finite state machine with the 
 *  following statuses: Initialisation, Pre-operational, Operational, Stopped.
 *  During power-up each node is in the Initialization state.
 *  At the end of this phase, it attempts to send a boot-up message. As soon as 
 *  it has been successfully sent, it is placed in the pre-operational state.
 *  Using an NMT master message, the VCU can make the various SCUs pass between
 *  the various Pre-operational, Operational and Stopped states.
 *
 *  Each SCU sends PDOs with sensor data in synchronous mode only if it is in 
 *  the Operational state.
 *
 *  @image html fsm_diagram.svg
 *  @image latex fsm_diagram.eps "FSM diagram" width=10cm
 */

/**
 *  @addtogroup CAN_network_module
 *   @{
 */

/**
 *  @defgroup CANopen_FSM_module CANopen Finite State Machine module
 *   @{
 */

/** FSM states enum */
enum enum_nodeState {
    Initialisation = 0x00,      /*!< Initialisation state */
    Disconnected = 0x01,        /*!< Disconnected state */
    Connecting = 0x02,          /*!< Connecting state */
    Preparing = 0x02,           /*!< Preparing state */
    Stopped = 0x04,             /*!< Stopped state */
    Operational = 0x05,         /*!< Operational state */
    Pre_operational = 0x7F,     /*!< PreOperational state */
    Unknown_state = 0x0F        /*!< Unknown state */
} __attribute__ ((packed));

/** FSM states typedef */
typedef enum enum_nodeState e_nodeState;

/**
 *  @brief      Initialisation state task on the FSM.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void initialisation();

/**
 *  @brief      preOperational state task on the FSM.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void preOperational();

/**
 *  @brief      Operational state task on the FSM.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void operational();

/**
 *  @brief      Stopped state task on the FSM.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void stopped();

/**
 *  @brief      Called by driver when receiving CANopen messages.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in]  m   Received CANopen message
 */
void canDispatch(Message* m);

/**
 *  @brief      Return current state on the @ref FSM_page
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @return     current state on the FSM
 */
e_nodeState getState();

/**
 *  @brief      Set current state on the @ref FSM_page
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in]  newState    New state transition
 */
void setState(e_nodeState newState);

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* _STATES_H_ */