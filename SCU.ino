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
#include "CarSensors.h"

#define TX_PERIOD       				1000 // 1ms
#define CAN_PORT        				Can0



uint8_t IT = 0; // Interval Timer Slot
DueTimer*    timer;

CarSensors sensors;

inline void CAN_pack_pedals(CAN_FRAME* frame) {
  frame -> id = PEDALS_ID;
  frame -> extended = false;
  frame -> data.byte[0] = sensors.getTps1Percentage();
  frame -> data.byte[1] = sensors.getTps2Percentage();
  frame -> data.byte[2] = sensors.getBrakePercentage();
}

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
	  //CAN_pack_accelerometers(&outgoing);
	  break;
	case 3:
	  //CAN_pack_suspensions(&outgoing);
	  break;
	default: IT = 0;
  }

  CAN_PORT.sendFrame(outgoing);
}

inline void CANInit() {
  // sync with VCU
  
  // if TCS is connected send all sensors data through CAN & radio
  
  // else send only pedals through CAN and all sensors data through radio
}

inline void TimerInit() {
  timer = &(DueTimer::getAvailable().attachInterrupt(timer_handler));
}

inline void RadioInit() {
  
}

inline void SCU_start() {
  timer -> start(TX_PERIOD);
}

void setup() {
  CANInit();
  RadioInit();
  TimerInit();

  SCU_start();
}

void loop() {
}
