/** 
 *  @file           states.cpp
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANopen finite state machine implementation file
 */

#include "timer.h"
#include "def.h"
#include "pdo.h"
#include "states.h"
#include "nmt.h"
#include "CO_can.h"

#include "common.h"
#include "model.h"

#if defined(_RETRO_)
#include "radio.h"
#endif

/**
 *  @addtogroup CANopen_FSM_module
 *   @{
 */

/**
 *  @var volatile e_nodeState current_state;
 *  @brief Current state of FSM.
 */
volatile e_nodeState current_state = Initialisation;

__attribute__((__inline__))
e_nodeState getState() {
	return current_state;
}

__attribute__((__inline__))
void setState(e_nodeState newState) {
	current_state = newState;
}

// gestisci solo comandi NMT e se SCU_R anche i PDO di SCU_F
void canDispatch(Message* m) {
	switch (GET_FUNC_CODE(m -> cob_id)) {
		case NMT:
            proceedNMTstateChange(m);
            break;
#if defined(_RETRO_)
        case PDO1rx:
        case PDO2rx:
            proceedPDO(m);
            break;
#endif
        default: {}
	}
}

/**
 *  @brief      Initialize @ref Board_model_group.
 *              If rear SCU firmware is selected (according to @ref SCU_firmware_selection)
 *              radio is initialized.
 *              It initializes the entire application, CAN/CANopen interfaces 
 *              and communication.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
__attribute__((__inline__))
void initialisation() {
    setState(Initialisation);
    model_init();

#if defined(_RETRO_)
    radio_init();
#endif
    initCAN();
    timerInit();
}

__attribute__((__inline__))
void preOperational() {
    setState(Pre_operational);
}

/**
 *  @brief      Start timer for periodic TPDO transmit according to 
 *              @ref CAN_network_page.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
__attribute__((__inline__))
void operational() {
    setState(Operational);
	timerStart();
}

/**
 *  @brief      Stop timer for periodic TPDO transmit according to 
 *              @ref CAN_network_page.
 *  
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
__attribute__((__inline__))
void stopped() {
    setState(Stopped);
	timerStop();
}

/**
 *  @}
 */