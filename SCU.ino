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

#include "PhonicWheels.h"

#define SERIAL_BAUDRATE         1000000
#define HOLES_NUMBER					  30
#define ENCODERS_FLUSH_PERIOD		256

void PhonicWheels_init() {
  PhonicWheels::Instance().getEncoder(RT_SX).attachPin(48);
  
  PhonicWheels::Instance().getEncoder(RT_SX).setCogsNumber(HOLES_NUMBER);

  #if !_USE_TIMING_REFERENCES_
  PhonicWheels::Instance().setFlushPeriod(ENCODERS_FLUSH_PERIOD);
  #endif
  
  PhonicWheels::Instance().begin();
}

void Serial_log() {
  Serial.print("RT_SX: ");
  Serial.print(PhonicWheels::Instance().getEncoder(RT_SX).getRPM());
}

void setup() {
  PhonicWheels_init();
  Serial.begin(SERIAL_BAUDRATE);
}

void loop() {
  Serial_log();
}
