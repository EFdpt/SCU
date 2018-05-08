#include "def.h"
#include "states.h"
#include "nmt.h"

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

void canDispatch(Message *m) {
	switch (GET_FUNC_CODE(m -> cob_id)) {
		case NMT:
            proceedNMTstateChange(m);
            break;
        default: {}
	}
}