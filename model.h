#ifndef _MODEL_H_
#define _MODEL_H_

#include <due_can.h>
#include "sensors_pinout.h"
#include "common.h"

#if defined(_FRONTAL_)
extern volatile uint16_t tps1_value;
extern volatile uint16_t tps1_max;
extern volatile uint16_t tps1_low;

extern volatile bool plaus;

extern volatile uint16_t tps2_value;
extern volatile uint16_t tps2_max;
extern volatile uint16_t tps2_low;

extern volatile uint16_t brake_value;
extern volatile uint16_t brake_max;
extern volatile uint16_t brake_low;

extern uint16_t fr_sx_rpm;
extern uint16_t fr_dx_rpm;

extern volatile uint16_t fr_sx_susp;
extern volatile uint16_t fr_dx_susp;

#elif defined(_RETRO_)
extern uint16_t rt_sx_rpm;
extern uint16_t rt_dx_rpm;

extern uint16_t acc_x_value;
extern uint16_t acc_y_value;

extern uint16_t rt_sx_susp;
extern uint16_t rt_dx_susp;
#else
  #error "No board specified"
#endif

void model_init();
void model_enable_calibrations();
void model_stop_calibrations();

void CAN_pack_model_data(CAN_FRAME* frame);

#endif /* _MODEL_H_ */
