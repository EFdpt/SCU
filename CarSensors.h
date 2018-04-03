#ifndef _CAR_SENSORS_
#define _CAR_SENSORS_

#include "sensors_pinout.h"
#include "PhonicWheels.h"

#define PEDALS_ID                     11
#define WHEELS_ID                     12

#define PW_HOLES_NUMBER               30
#define PHONIC_WHEELS_FLUSH_PERIOD    256

class CarSensors {
	private:
		volatile uint16_t tps1_value;
    volatile uint16_t tps1_min;
    volatile uint16_t tps1_max;
    
		volatile uint16_t tps2_value;
    volatile uint16_t tps2_min;
    volatile uint16_t tps2_max;
   
		volatile uint16_t brake_value;
    volatile uint16_t brake_min;
    volatile uint16_t brake_max;
    
		volatile uint16_t acc_x_value;
		volatile uint16_t acc_y_value;
		volatile uint16_t fr_sx_susp_value;
		volatile uint16_t fr_dx_susp_value;
		volatile uint16_t rt_sx_susp_value;
		volatile uint16_t rt_dx_susp_value;

   volatile bool calibration_enabled;

	public:
    CarSensors();

    volatile uint8_t getTps1Percentage();
		volatile uint16_t getTps1Value();
    volatile uint8_t getTps2Percentage();
		volatile uint16_t getTps2Value();
    volatile uint8_t getBrakePercentage();
		volatile uint16_t getBrakeValue();

    void enable_calibration();
    void disable_calibration();
/*
		volatile uint16_t brake_value;
		volatile uint16_t acc_x_value;
		volatile uint16_t acc_y_value;
		volatile uint16_t fr_sx_susp_value;
		volatile uint16_t fr_dx_susp_value;
		volatile uint16_t rt_sx_susp_value;
		volatile uint16_t rt_dx_susp_value;*/
		PhonicWheels getPhonicWheels();
};

#endif /* _CAR_SENSORS_ */
