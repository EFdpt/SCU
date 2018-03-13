/**
 * @mainpage Sensors Control Unit (SCU) firmware
 * 
 * 		a
 * 
 * @section Phonic Wheel Encoders
 * 		
 * 
 * @section 
 * 
 * 
 * 
 * 
 * 
 */

#include <due_can.h>
#include <DueTimer.h>
#include "PhonicWheels.h"

#define TX_PERIOD       				1000 // 1ms
#define CAN_PORT        				Can0


#define HOLES_NUMBER					30
#define PHONIC_WHEELS_FLUSH_PERIOD		256
#define WHEELS_ID						0x11

uint8_t IT = 0; // Interval Timer Slot

inline void CAN_pack_encoders(CAN_FRAME* frame) {
	frame -> id = WHEELS_ID;
	frame -> extended = false;
	frame -> data.s0 = PhonicWheels::Instance().getEncoder(FR_SX).getRPM();
	frame -> data.s1 = PhonicWheels::Instance().getEncoder(FR_DX).getRPM();
	frame -> data.s2 = PhonicWheels::Instance().getEncoder(RT_SX).getRPM();
	frame -> data.s3 = PhonicWheels::Instance().getEncoder(RT_DX).getRPM();
}

void timer_handler() {
  // send packets
  CAN_FRAME     outgoing;

  switch (IT++) {
	case 0:
	  CAN_pack_pedals(&outgoing);
	  break;
	case 1:
	  CAN_pack_encoders(&outgoing);
	  break;
	case 2:
	  CAN_pack_accelerometers(&outgoing);
	  break;
	case 3:
	  CAN_pack_suspensions(&outgoing);
	  break;
	default: IT = 0;
  }

  CAN_PORT.sendFrame(outgoing);
}

inline void CAN_init() {
  // sync with VCU
  
  // if TCS is connected send all sensors data through CAN & radio
  
  // else send only pedals through CAN and all sensors data through radio
}

inline void Timer_init() {
  DueTimer::getAvailable().attachInterrupt(timer_handler).start(TX_PERIOD);
}

void PhonicWheels_init() {
  PhonicWheels::setFlushPeriod(PHONIC_WHEELS_FLUSH_PERIOD);
  PhonicWheels::Instance().getEncoder(FR_SX).attachPin(0);
  PhonicWheels::Instance().getEncoder(FR_DX).attachPin(1);
  PhonicWheels::Instance().getEncoder(RT_SX).attachPin(2);
  PhonicWheels::Instance().getEncoder(RT_DX).attachPin(3);
  PhonicWheels::Instance().getEncoder(FR_SX).setHolesNumber(HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(FR_DX).setHolesNumber(HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(RT_SX).setHolesNumber(HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(RT_DX).setHolesNumber(HOLES_NUMBER);
  PhonicWheels::Instance().begin();
}

inline void Sensors_init() {
  Pedals_init();
  
  PhonicWheels_init();
}

void setup() {
  CAN_init();
  Radio_init();
  Timer_init();
  Sensors_init();
}

void loop() {
}
