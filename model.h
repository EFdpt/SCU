#ifndef _MODEL_H_
#define _MODEL_H_

#include "sensors_pinout.h"
#include "common.h"

#if !defined(_FRONTAL_) && !defined(_RETRO_)
  #error "No board specified"
#endif

#define SUSP_STROKE_EXTENSION	75.0	// [mm]

//extern volatile uint16_t tps1_value;
extern volatile uint8_t  tps1_percentage;
//extern volatile uint16_t tps1_max;
//extern volatile uint16_t tps1_low;

extern volatile bool     apps_plausibility;
extern volatile bool     brake_plausibility;

//extern volatile uint16_t tps2_value;
extern volatile uint8_t  tps2_percentage;
//extern volatile uint16_t tps2_max;
//extern volatile uint16_t tps2_low;

//extern volatile uint16_t brake_value;
extern volatile uint8_t  brake_percentage;
//extern volatile uint16_t brake_max;
//extern volatile uint16_t brake_low;

extern volatile uint8_t fr_sx_susp;
extern volatile uint8_t fr_dx_susp;

extern volatile uint16_t fr_sx_rpm;
extern volatile uint16_t fr_dx_rpm;

volatile uint16_t get_fr_sx_rpm();
volatile uint16_t get_fr_dx_rpm();

#if defined(_RETRO_)
extern volatile uint8_t acc_x_value;
extern volatile uint8_t acc_z_value;

extern volatile uint8_t rt_sx_susp;
extern volatile uint8_t rt_dx_susp;

volatile uint16_t get_rt_sx_rpm();
volatile uint16_t get_rt_dx_rpm();
#endif

void model_init();

#if defined(_FRONTAL_)
void model_enable_calibrations();
void model_stop_calibrations();
#endif

#endif /* _MODEL_H_ */
