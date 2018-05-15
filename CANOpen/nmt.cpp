#include "nmt.h"
#include "states.h"

void proceedNMTstateChange(Message* m) {
	if (m -> data[1] == 0 || m -> data[1] == getNodeId()) {
		switch (m -> data[0]) { /* command specifier (cs) */
            case NMT_Start_Node:
                if ((getState() == Pre_operational) || (getState() == Stopped))
                    setState(Operational);
                    operational();
                break;

            case NMT_Stop_Node:
                if (getState() == Pre_operational || getState() == Operational)
                    setState(Stopped);
                    stopped();
                break;

            case NMT_Enter_PreOperational:
                if (getState() == Operational || getState() == Stopped)
                    setState(Pre_operational);
                    preOperational();
                break;

            case NMT_Reset_Node:
          		setState(Initialisation);
                initialisation();
                break;
            default: {}
		}
	}
}

void slaveSendBootUp() {
	Message m;

	m.cob_id = 0x700 | getNodeId();
	m.length = 1;
	m.data[0] = 0x00;

	canSend(&m);
}