#include "model.h"
#include "filter.h"

#undef HID_ENABLED
#define BUFFER_SIZE     128
#define BUFFERS         4

volatile int bufn, obufn;
volatile bool calibrate;

#if defined(_FRONTAL_)

#define ADC_CHANNELS        5
#define ADC_MIN             0
#define ADC_MAX             4095 / ADC_CHANNELS

uint16_t buf[BUFFERS][BUFFER_SIZE * ADC_CHANNELS];

uint16_t tps1_value;
uint16_t tps1_max = ADC_MIN;
uint16_t tps1_low = ADC_MAX;

uint16_t tps2_value;
uint16_t tps2_max = ADC_MIN;
uint16_t tps2_low = ADC_MAX;

uint16_t brake_value;
uint16_t brake_max = ADC_MIN;
uint16_t brake_low = ADC_MAX;

uint16_t fr_sx_rpm;
uint16_t fr_dx_rpm;

uint16_t fr_sx_susp;
uint16_t fr_dx_susp;

void ADC_Handler() {     // move DMA pointers to next buffer
  int f = ADC->ADC_ISR;
  if (f & (1<<27)){
    bufn = (bufn + 1) &(BUFFERS - 1);
    ADC->ADC_RNPR = (uint32_t) buf[bufn];
    ADC->ADC_RNCR = BUFFER_SIZE * ADC_CHANNELS;

    // filter acquired data
    tps1_value = (tps1_value + filter_buffer(buf[obufn], BUFFER_SIZE, ADC_CHANNELS)) / 2;
    tps2_value = (tps2_value + filter_buffer(buf[obufn] + 1, BUFFER_SIZE, ADC_CHANNELS)) / 2; // gestire l'offset
    brake_value = (brake_value + filter_buffer(buf[obufn] + 2, BUFFER_SIZE, ADC_CHANNELS)) / 2;
    fr_sx_susp = (fr_sx_susp + filter_buffer(buf[obufn] + 3, BUFFER_SIZE, ADC_CHANNELS)) / 2;
    fr_dx_susp = (fr_dx_susp + filter_buffer(buf[obufn] + 4, BUFFER_SIZE, ADC_CHANNELS)) / 2;

    if (calibrate) {
      if (tps1_value < tps1_low) tps1_low = tps1_value;
      if (tps1_value > tps1_max) tps1_max = tps1_value;
      if (tps2_value < tps2_low) tps2_low = tps2_value;
      if (tps2_value > tps2_max) tps2_max = tps2_value;
      if (brake_value < brake_low) brake_low = brake_value;
      if (brake_value > brake_max) brake_max = brake_value;
    }
    
    obufn = (obufn + 1) &(BUFFERS - 1);
  }
}

void model_init() {
  pmc_enable_periph_clk(ID_ADC);
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  adc_set_resolution(ADC, ADC_12_BITS);
  ADC->ADC_MR |=0x80; // free running
/*
  ADC->ADC_CHER = ADC_CHANNEL_ENABLE(TPS1_ADC_CHAN_NUM) |
                  ADC_CHANNEL_ENABLE(TPS2_ADC_CHAN_NUM) |
                  ADC_CHANNEL_ENABLE(BRAKE_ADC_CHAN_NUM) |
                  ADC_CHANNEL_ENABLE(FR_SX_ADC_CHAN_NUM) |
                  ADC_CHANNEL_ENABLE(FR_DX_ADC_CHAN_NUM);*/
  ADC->ADC_CHER = ADC_CHER_CH7 | ADC_CHER_CH6 | ADC_CHER_CH5 | ADC_CHER_CH4 | ADC_CHER_CH3;

  NVIC_EnableIRQ(ADC_IRQn);
  ADC->ADC_IDR = ~ADC_IDR_ENDRX; //~(1<<27); End of Receive Buffer Interrupt Disable
  ADC->ADC_IER = ADC_IDR_ENDRX; // 1<<27; End of Receive Buffer Interrupt Enable
  ADC->ADC_RPR = (uint32_t) buf[0];   // DMA buffer
  ADC->ADC_RCR = BUFFER_SIZE * ADC_CHANNELS;
  ADC->ADC_RNPR = (uint32_t) buf[1]; // next DMA buffer
  ADC->ADC_RNCR = BUFFER_SIZE * ADC_CHANNELS;
  bufn = obufn = 1;
  ADC->ADC_PTCR = 1;
  ADC->ADC_CR = 2;
}

#elif defined(_RETRO_)

#define ADC_CHANNELS        4
#define ADC_MIN             0
#define ADC_MAX             4095 / ADC_CHANNELS

uint16_t rt_sx_rpm;
uint16_t rt_dx_rpm;

uint8_t acc_x_value;
uint8_t acc_y_value;

uint8_t rt_sx_susp;
uint8_t rt_dx_susp;

#endif

void model_enable_calibrations() {
  calibrate = true;
}
void model_stop_calibrations() {
  calibrate = false;
}

void CAN_pack_model_data(CAN_FRAME* frame) {
  switch (frame -> id) {
  #if defined(_FRONTAL_)
    case CAN_PEDALS_ID:
      frame -> length = 3;
      frame -> data.byte[0] = (uint8_t) map(tps1_value, tps1_low, tps1_max, ADC_MIN, ADC_MAX);
      frame -> data.byte[1] = (uint8_t) map(tps2_value, tps2_low, tps2_max, ADC_MIN, ADC_MAX);
      frame -> data.byte[2] = (uint8_t) map(brake_value, brake_low, brake_max, ADC_MIN, ADC_MAX);
      break;
    case CAN_FRONTAL_ID:
      frame -> length = 6;

      // TODO
      break;
  #elif defined(_RETRO_)
      // TODO
  #endif
  }
}
