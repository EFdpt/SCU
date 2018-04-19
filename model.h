#ifndef _MODEL_H_
#define _MODEL_H_

#include <due_can.h>
#include "sensors_pinout.h"
#include "common.h"

#if !defined(_FRONTAL_) && !defined(_RETRO_)
  #error "No board specified"
#endif

extern volatile bool 	 tcs_online;

//extern volatile uint16_t tps1_value;
extern volatile uint8_t  tps1_percentage;
extern volatile uint16_t tps1_max;
extern volatile uint16_t tps1_low;

extern volatile bool     plaus1;
extern volatile bool     plaus2;

//extern volatile uint16_t tps2_value;
extern volatile uint8_t  tps2_percentage;
extern volatile uint16_t tps2_max;
extern volatile uint16_t tps2_low;

//extern volatile uint16_t brake_value;
extern volatile uint8_t  brake_percentage;
extern volatile uint16_t brake_max;
extern volatile uint16_t brake_low;

extern volatile uint8_t fr_sx_susp;
extern volatile uint8_t fr_dx_susp;

extern volatile uint8_t acc_x_value;
extern volatile uint8_t acc_y_value;

extern volatile uint8_t rt_sx_susp;
extern volatile uint8_t rt_dx_susp;

volatile uint16_t get_fr_sx_rpm();
volatile uint16_t get_fr_dx_rpm();
volatile uint16_t get_rt_sx_rpm();
volatile uint16_t get_rt_dx_rpm();

void model_init();
void model_enable_calibrations();
void model_stop_calibrations();

void CAN_pack_model_data(CAN_FRAME* frame);
void CAN_unpack_model_data(CAN_FRAME* frame);

void RADIO_send_model();

#endif /* _MODEL_H_ */
