#include "CO_can.h"

#include <due_can.h>

Can* can_port;

void canSend(Message* m) {
	CAN_FRAME frame;

	frame.id = m -> cob_id;
	frame.length = m -> length;
	frame.extended = false;
	memcpy(frame.data, m -> data, m -> length);

	can_port.sendFrame(frame);
}

void initCAN(Can* p_can, uint32_t baudRate) {
	can_port = p_can;
	can_port.begin(baudRate);
	can_port.init_Mask(0,0, 0x007E); // allow two consecutive ids
  CAN.init_Filt(0,0, getNodeId()); // RxPDO and SDO
  CAN.init_Filt(1,0, getNodeId()); // same

  CAN.init_Mask(1,0, 0x07FF);
  CAN.init_Filt(2,0, 0x0000); // NMT
  
  CAN.init_Filt(5,0, 0x0000); // dummy
}