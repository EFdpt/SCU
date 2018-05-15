#include <due_can.h>

#include "CO_can.h"
#include "states.h"
#include "../common.h"

void canSend(Message* m) {
	CAN_FRAME frame;

	frame.id = m -> cob_id;
	frame.length = m -> length;
	frame.extended = false;
	memcpy(frame.data, m -> data, m -> length);

	can_port.sendFrame(frame);
}

void CAN_general_callback(CAN_FRAME* frame) {
    Message m = Message_Initializer;

    // copy data from "datalink layer" to "application layer"
    m.cob_id = (uint16_t) frame -> id;
    m.len = frame -> length;
    memcpy(m.data, frame -> data, m.len);

    canDispatch(&m);
}

void initCAN() {
    uint32_t i = 0;

    while (!CAN_PORT.begin(CAN_BAUDRATE)) {}

    // set [0..3] mailboxes in RX mode with 0 filter and 0 mask (accept any frame)
    for (; i < 4; i++) {
        CAN_PORT.setRXFilter(i, 0, 0, false);
        CAN_PORT.setCallback(i, CAN_general_callback);
    }

    // set [4..7] mailboxes in TX mode
    for (; i < 8; i++)
        CAN_PORT.mailbox_set_mode(i, CAN_MB_TX_MODE);
}