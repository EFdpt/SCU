/** 
 *  @file           nmt.cpp
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANOpen NMT module implementation file
 */

#include "def.h"
#include "nmt.h"
#include "states.h"

/** @addtogroup CANopen_NMT_group
 *   @{
 */

/**
 *  @brief      According to @ref CANopen_NMT_speficications, upon NMT reception
 *              from VCU master node, SCU change current state.
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @param[in] m    Received NMT message
 */
void proceedNMTstateChange(Message* m) {
	if (m -> data[1] == 0 || m -> data[1] == getNodeId()) {
		switch (m -> data[0]) { /* command specifier (cs) */
            case NMT_Start_Node:
                if ((getState() == Pre_operational) || (getState() == Stopped))
                    operational();
                break;

            case NMT_Stop_Node:
                if (getState() == Pre_operational || getState() == Operational)
                    stopped();
                break;

            case NMT_Enter_PreOperational:
                if (getState() == Operational || getState() == Stopped)
                    preOperational();
                break;

            case NMT_Reset_Node:
          		initialisation();
                break;
            default: {}
		}
	}
}

/**
 *  @brief      This function sends a slave boot-up message over CAN servizi network.
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
__attribute__((__inline__))
void slaveSendBootUp() {
	Message m;

	m.cob_id = 0x700 | getNodeId();
	m.len = 1;
	m.data[0] = 0x00;

	canSend(&m);
}

/**
 *  @}
 */