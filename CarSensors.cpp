#include "CarSensors.h"

#define INFINITE      0xFFFF

CarSensors::CarSensors() {
  analogReadResolution(12);
  PhonicWheels::Instance().getEncoder(FR_SX).attachPin(FR_SX_PW_PIN);
  PhonicWheels::Instance().getEncoder(FR_DX).attachPin(FR_DX_PW_PIN);
  PhonicWheels::Instance().getEncoder(RT_SX).attachPin(RT_SX_PW_PIN);
  PhonicWheels::Instance().getEncoder(RT_DX).attachPin(RT_DX_PW_PIN);
  PhonicWheels::Instance().getEncoder(FR_SX).setHolesNumber(PW_HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(FR_DX).setHolesNumber(PW_HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(RT_SX).setHolesNumber(PW_HOLES_NUMBER);
  PhonicWheels::Instance().getEncoder(RT_DX).setHolesNumber(PW_HOLES_NUMBER);
  PhonicWheels::Instance().setFlushPeriod(PHONIC_WHEELS_FLUSH_PERIOD);
  PhonicWheels::Instance().begin();

  this -> tps1_min = 0;
  this -> tps1_max = INFINITE;
  this -> tps2_min = 0;
  this -> tps2_max = INFINITE;
  this -> brake_min = 0;
  this -> brake_max = INFINITE;

  this -> calibration_enabled = true;
}
    
volatile uint16_t CarSensors::getTps1Value() {
  this -> tps1_value = analogRead(TPS1_PIN);
  if (this -> calibration_enabled) {
    if (this -> tps1_value < this -> tps1_min)
      this -> tps1_min = this -> tps1_value;
    if (this -> tps1_value > this -> tps1_max)
      this -> tps1_max = this -> tps1_value;
  }
  return this -> tps1_value;
}

volatile uint8_t CarSensors::getTps1Percentage() {
  return map(this -> getTps1Value(), this -> tps1_min, this -> tps1_max, 0, 4095);
}

volatile uint16_t CarSensors::getTps2Value() {
  this -> tps2_value = analogRead(TPS2_PIN);
  if (this -> calibration_enabled) {
    if (this -> tps2_value < this -> tps2_min)
      this -> tps2_min = this -> tps2_value;
    if (this -> tps2_value > this -> tps2_max)
      this -> tps2_max = this -> tps2_value;
  }
  return this -> tps2_value;
}

volatile uint8_t CarSensors::getTps2Percentage() {
  return map(this -> getTps2Value(), this -> tps2_min, this -> tps2_max, 0, 4095);
}

volatile uint16_t CarSensors::getBrakeValue() {
  this -> brake_value = analogRead(BRAKE_PIN);
  if (this -> calibration_enabled) {
    if (this -> brake_value < this -> brake_min)
      this -> brake_min = this -> brake_value;
    if (this -> brake_value > this -> brake_max)
      this -> brake_max = this -> brake_value;
  }
  return this -> brake_value;
}

volatile uint8_t CarSensors::getBrakePercentage() {
  return map(this -> getBrakeValue(), this -> brake_min, this -> brake_max, 0, 4095);
}

void CarSensors::enable_calibration() {
  this -> calibration_enabled = true;
}

void CarSensors::disable_calibration() {
  this -> calibration_enabled = false;
}
