#include "timer.h"
#include "def.h"
#include "states.h"
#include "nmt.h"
#include "CO_can.h"

e_nodeState current_state = Initialisation;
uint8_t 	nodeId;

inline e_nodeState getState() {
	return current_state;
}

inline void setState(e_nodeState newState) {
	current_state = newState;
}

inline uint8_t getNodeId() {
	return nodeId;
}

inline void setNodeId(uint8_t nodeId) {
	nodeId = nodeId;
}

// gestisci solo comandi NMT e se SCU_R anche i PDO di SCU_F
void canDispatch(Message *m) {
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

void initialisation() {
    model_init();
#if defined(_FRONTAL_)
    model_enable_calibrations();
#endif
    initCAN();
    timerInit();
}

void preOperational() {}

void operational() {
	timerStart();
}

void stopped() {
	timerStop();
}