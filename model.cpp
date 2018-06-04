#include "model.h"
#include "filter.h"

#undef HID_ENABLED
#define ADC_BUFFER_SIZE       128
#define BUFFERS               4

#define ADC_MIN               0
#define ADC_MAX               4095

#define SUSPENSIONS_MIN       0   /* [mm] */
#define SUSPENSIONS_ADC_MAX   ADC_MAX
#define SUSP_STROKE_NORMALIZE (SUSP_STROKE_EXTENSION / SUSPENSIONS_ADC_MAX) /* normalize voltage */

#define COGS_NUMBER           30.0d
#define NORMALIZE_RPM         1000000.0d
#define RPM_MIN               10

#define APPS_PLAUS_RANGE      10

#if defined(_FRONTAL_)

  #define ADC_CHANNELS        5
  #define ADC_CHANNELS_LIST   TPS1_ADC_CHAN_NUM | TPS2_ADC_CHAN_NUM | BRAKE_ADC_CHAN_NUM | FR_SX_ADC_CHAN_NUM | FR_DX_ADC_CHAN_NUM
  
  #define BUFFER_LENGTH       ADC_BUFFER_SIZE * ADC_CHANNELS    
  
#elif defined(_RETRO_)

  #define ADC_CHANNELS        4
  #define ADC_CHANNELS_LIST   ACC_X_ADC_CHAN_NUM | ACC_Z_ADC_CHAN_NUM | RT_SX_ADC_CHAN_NUM | RT_DX_ADC_CHAN_NUM
  
  #define BUFFER_LENGTH       ADC_BUFFER_SIZE * ADC_CHANNELS    
  
#endif

volatile int      bufn, obufn;

#if defined(_FRONTAL_)
volatile bool     calibrate = true;   // calibrate pedals
#endif

#if defined(_FRONTAL_)
volatile uint16_t tps1_value = 0;
volatile uint16_t tps2_value = 0;
volatile uint16_t brake_value = 0;
#endif

volatile uint8_t  tps1_percentage = 0; // global
volatile uint16_t tps1_max = ADC_MIN;
volatile uint16_t tps1_low = ADC_MAX;

volatile bool     apps_plausibility = true; // global
volatile bool     brake_plausibility = true; // global

volatile uint8_t  tps2_percentage = 0; // global
volatile uint16_t tps2_max = ADC_MIN;
volatile uint16_t tps2_low = ADC_MAX;

volatile uint8_t  brake_percentage = 0; // global
volatile uint16_t brake_max = ADC_MIN;
volatile uint16_t brake_low = ADC_MAX;

volatile uint16_t fr_sx_rpm = 0; // global
volatile uint16_t fr_dx_rpm = 0; // global

#if defined(_FRONTAL_)

// phonic wheel encoders timings
volatile unsigned long fr_sx_prev;
volatile unsigned long fr_sx_curr;

volatile unsigned long fr_dx_prev;
volatile unsigned long fr_dx_curr;
#endif

// FRONTAL SUSPENSIONS
volatile uint8_t fr_sx_susp = 0;
volatile uint8_t fr_dx_susp = 0;

#if defined(_RETRO_)

volatile uint16_t rt_sx_rpm = 0;
volatile unsigned long rt_sx_prev;
volatile unsigned long rt_sx_curr;

volatile uint16_t rt_dx_rpm = 0;
volatile unsigned long rt_dx_prev;
volatile unsigned long rt_dx_curr;
  
volatile uint8_t acc_x_value = 0;
volatile uint8_t acc_z_value = 0;
  
volatile uint8_t rt_sx_susp = 0;
volatile uint8_t rt_dx_susp = 0;

#endif

volatile uint16_t buf[BUFFERS][BUFFER_LENGTH];

// retrieve wheels RPM
#if defined(_FRONTAL_)

void fr_sx_pulse() {
  fr_sx_prev = fr_sx_curr;
  fr_sx_curr = micros();
}

void fr_dx_pulse() {
  fr_dx_prev = fr_dx_curr;
  fr_dx_curr = micros();
}

__attribute__((__inline__)) volatile uint16_t get_fr_sx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (fr_sx_curr - fr_sx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

__attribute__((__inline__)) volatile uint16_t get_fr_dx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (fr_dx_curr - fr_dx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

#elif defined(_RETRO_)

void rt_sx_pulse() {
  rt_sx_prev = rt_sx_curr;
  rt_sx_curr = micros();
}

void rt_dx_pulse() {
  rt_dx_prev = rt_dx_curr;
  rt_dx_curr = micros();
}

// frontal rpm values getted from CAN frame
__attribute__((__inline__)) volatile uint16_t get_fr_sx_rpm() {
  return fr_sx_rpm;
}

__attribute__((__inline__)) volatile uint16_t get_fr_dx_rpm() {
  return fr_dx_rpm;
}

__attribute__((__inline__)) volatile uint16_t get_rt_sx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (rt_sx_curr - rt_sx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

__attribute__((__inline__)) volatile uint16_t get_rt_dx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (rt_dx_curr - rt_dx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

#endif

static inline void filter_data() {
  #if defined(_FRONTAL_)
    tps1_value = (tps1_value + filter_buffer(buf[obufn] + TPS1_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;
    tps2_value = (tps2_value + filter_buffer(buf[obufn] + TPS2_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;
    brake_value = (brake_value + filter_buffer(buf[obufn] + BRAKE_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;

    fr_sx_susp = (volatile uint8_t) (fr_sx_susp + (SUSP_STROKE_NORMALIZE * filter_buffer(buf[obufn] + FR_SX_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;
    fr_dx_susp = (volatile uint8_t) (fr_dx_susp + (SUSP_STROKE_NORMALIZE * filter_buffer(buf[obufn] + FR_DX_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;

    if (calibrate) {
      if (tps1_value < tps1_low) tps1_low = tps1_value;
      if (tps1_value > tps1_max) tps1_max = tps1_value;
      if (tps2_value < tps2_low) tps2_low = tps2_value;
      if (tps2_value > tps2_max) tps2_max = tps2_value;
      if (brake_value < brake_low) brake_low = brake_value;
      if (brake_value > brake_max) brake_max = brake_value;
    }

    tps1_percentage = map(tps1_value, tps1_low, tps1_max, 0, 100);
    tps2_percentage = map(tps2_value, tps2_low, tps2_max, 0, 100);
    brake_percentage = map(brake_value, brake_low, brake_max, 0, 100);

    // check APPS plausibility
    if (abs(tps1_percentage - tps2_percentage) > APPS_PLAUS_RANGE)
      apps_plausibility = false;
    else
      apps_plausibility = true;

    // check APPS + brake plausibility (BSPD)
    if (tps1_percentage > 5 && brake_percentage > 25) // ACCELERATOR + BRAKE plausibility
      brake_plausibility = false;
    else if (!brake_percentage)
      brake_plausibility = true;
    
  #elif defined(_RETRO_)
    rt_sx_susp = (volatile uint8_t) (rt_sx_susp + (SUSP_STROKE_NORMALIZE * filter_buffer(buf[obufn] + RT_SX_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;
    rt_dx_susp = (volatile uint8_t) (rt_dx_susp + (SUSP_STROKE_NORMALIZE * filter_buffer(buf[obufn] + RT_DX_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;
  #endif
}

void ADC_Handler() {
  int f = ADC->ADC_ISR;
  if (f & (1 << 27)){
    bufn = (bufn + 1) &(BUFFERS - 1); // move DMA pointers to next buffer
    ADC->ADC_RNPR = (uint32_t) buf[bufn];
    ADC->ADC_RNCR = BUFFER_LENGTH;

    filter_data();
    
    obufn = (obufn + 1) &(BUFFERS - 1);
  }
}

void model_init() {
  pmc_enable_periph_clk(ID_ADC);
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  adc_set_resolution(ADC, ADC_12_BITS);
  ADC->ADC_MR |=0x80; // free running

  ADC->ADC_CHER = ADC_CHANNELS_LIST;

  NVIC_EnableIRQ(ADC_IRQn);
  ADC->ADC_IDR = ~ADC_IDR_ENDRX; //~(1<<27); End of Receive Buffer Interrupt Disable
  ADC->ADC_IER = ADC_IDR_ENDRX; // 1<<27; End of Receive Buffer Interrupt Enable
  ADC->ADC_RPR = (uint32_t) buf[0];   // DMA buffer
  ADC->ADC_RCR = BUFFER_LENGTH;
  ADC->ADC_RNPR = (uint32_t) buf[1]; // next DMA buffer
  ADC->ADC_RNCR = BUFFER_LENGTH;
  bufn = obufn = 1;
  ADC->ADC_PTCR = 1;
  ADC->ADC_CR = 2;

  #if defined(_FRONTAL_)
  fr_sx_curr = micros();
  fr_dx_curr = micros();
  attachInterrupt(digitalPinToInterrupt(FR_SX_PW_PIN), fr_sx_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(FR_DX_PW_PIN), fr_dx_pulse, RISING);
  #elif defined(_RETRO_)
  rt_sx_curr = micros();
  rt_dx_curr = micros();
  attachInterrupt(digitalPinToInterrupt(RT_SX_PW_PIN), rt_sx_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(RT_DX_PW_PIN), rt_dx_pulse, RISING);
  #endif
}

#if defined(_FRONTAL_)
__attribute__((__inline__)) void model_enable_calibrations() {
  calibrate = true;
}
__attribute__((__inline__)) void model_stop_calibrations() {
  calibrate = false;
}
#endif
