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

// C:\ProgramData\MATLAB\SupportPackages\R2017b\3P.instrset\arduinoide.instrset\idepkgs\packages\arduino\hardware\sam\1.6.11\system\libsam\include

#include <due_can.h>
#include <DueTimer.h>
#include "PhonicWheels.h"

#define SERIAL_BAUDRATE         1000000
#define CAN_BAUDRATE            1000000
#define TX_PERIOD       				1000 // 1ms
#define CAN_PORT        				Can0


#define HOLES_NUMBER					  30
#define ENCODERS_FLUSH_PERIOD		256 // 


#define WHEELS_ID						    0x11

#if 0
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

inline void Sensors_init() {
  Pedals_init();
  
  PhonicWheels_init();
}

inline void CAN_init() {
  // sync with VCU
  
  // if TCS is connected send all sensors data through CAN & radio
  
  // else send only pedals through CAN and all sensors data through radio
}

inline void Timer_init() {
  DueTimer::getAvailable().attachInterrupt(timer_handler).start(TX_PERIOD);
}

#endif

void PhonicWheels_init() {
  PhonicWheels::Instance().getEncoder(FR_SX).attachPin(0);
  PhonicWheels::Instance().getEncoder(FR_DX).attachPin(1);
  PhonicWheels::Instance().getEncoder(RT_SX).attachPin(2);
  PhonicWheels::Instance().getEncoder(RT_DX).attachPin(3);
  PhonicWheels::Instance().getEncoder(FR_SX).setCogsNumber(HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(FR_DX).setCogsNumber(HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(RT_SX).setCogsNumber(HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(RT_DX).setCogsNumber(HOLES_NUMBER);

  #if !_USE_TIMING_REFERENCES_
  PhonicWheels::Instance().getEncoder(FR_SX).setFlushPeriod(ENCODERS_FLUSH_PERIOD);
  PhonicWheels::Instance().getEncoder(FR_DX).setFlushPeriod(ENCODERS_FLUSH_PERIOD);
  PhonicWheels::Instance().getEncoder(RT_SX).setFlushPeriod(ENCODERS_FLUSH_PERIOD);
  PhonicWheels::Instance().getEncoder(RT_DX).setFlushPeriod(ENCODERS_FLUSH_PERIOD);
  #endif
  
  PhonicWheels::Instance().begin();
}

void Serial_log() {
  Serial.print("FR_SX: ");
  Serial.print(PhonicWheels::Instance().getEncoder(FR_SX).getRPM());
  Serial.print(", FR_DX: ");
  Serial.print(PhonicWheels::Instance().getEncoder(FR_DX).getRPM());
  Serial.print(", RT_SX: ");
  Serial.print(PhonicWheels::Instance().getEncoder(RT_SX).getRPM());
  Serial.print(", RT_DX: ");
  Serial.println(PhonicWheels::Instance().getEncoder(RT_DX).getRPM());
}

void setup() {
  #if 0
  CAN_init();
  Radio_init();
  Timer_init();
  Sensors_init();
  #endif
  PhonicWheels_init();
  Serial.begin(SERIAL_BAUDRATE);
}

void loop() {
  Serial_log();
}
