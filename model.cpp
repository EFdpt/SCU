#include "model.h"
#include "filter.h"

#if defined(_RETRO_)
#include <ArduinoJson.h>
#endif

#undef HID_ENABLED
#define ADC_BUFFER_SIZE       128
#define BUFFERS               4

#define ADC_MIN               0
#define ADC_MAX               4095

#define SUSPENSIONS_MIN       0   /* [mm] */
#define SUSPENSIONS_MAX       150 /* [mm] */

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
  #define ADC_CHANNELS_LIST   ACC_X_ADC_CHAN_NUM | ACC_Y_ADC_CHAN_NUM | RT_SX_ADC_CHAN_NUM | RT_DX_ADC_CHAN_NUM
  
  #define BUFFER_LENGTH       ADC_BUFFER_SIZE * ADC_CHANNELS    
  
#endif

volatile int      bufn, obufn;

volatile bool     tcs_online = false;

#if defined(_FRONTAL_)
volatile bool     calibrate = true;   // calibrate pedals
#endif

#if defined(_FRONTAL_)
volatile uint16_t tps1_value = 0;
volatile uint16_t tps2_value = 0;
volatile uint16_t brake_value = 0;
#endif

volatile uint8_t  tps1_percentage = 0;
volatile uint16_t tps1_max = ADC_MIN;
volatile uint16_t tps1_low = ADC_MAX;

volatile bool     plaus1 = true;
volatile bool     plaus2 = true;

volatile uint8_t  tps2_percentage = 0;
volatile uint16_t tps2_max = ADC_MIN;
volatile uint16_t tps2_low = ADC_MAX;

volatile uint8_t  brake_percentage = 0;
volatile uint16_t brake_max = ADC_MIN;
volatile uint16_t brake_low = ADC_MAX;

volatile uint16_t fr_sx_rpm = 0;
volatile uint16_t fr_dx_rpm = 0;

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
volatile uint8_t acc_y_value = 0;
  
volatile uint8_t rt_sx_susp = 0;
volatile uint8_t rt_dx_susp = 0;

#endif

volatile uint16_t buf[BUFFERS][BUFFER_LENGTH];

#if defined(_RETRO_)

#define JSON_BUFFER_SIZE      JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(11)

#endif

#if defined(_FRONTAL_)

inline volatile uint16_t get_fr_sx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (fr_sx_curr - fr_sx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

inline volatile uint16_t get_fr_dx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (fr_dx_curr - fr_dx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

#elif defined(_RETRO_)

// frontal rpm values getted from CAN frame
inline volatile uint16_t get_fr_sx_rpm() {
  return fr_sx_rpm;
}

inline volatile uint16_t get_fr_dx_rpm() {
  return fr_dx_rpm;
}

#endif

inline volatile uint16_t get_rt_sx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (rt_sx_curr - rt_sx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

inline volatile uint16_t get_rt_dx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (rt_dx_curr - rt_dx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

static void filter_data() {
  #if defined(_FRONTAL_)
    tps1_value = (tps1_value + filter_buffer(buf[obufn], ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;
    tps2_value = (tps2_value + filter_buffer(buf[obufn] + 1, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;
    brake_value = (brake_value + filter_buffer(buf[obufn] + 2, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;
    fr_sx_susp = (volatile uint8_t) (fr_sx_susp + map(filter_buffer(buf[obufn] + 3, ADC_BUFFER_SIZE, ADC_CHANNELS), ADC_MIN, ADC_MAX, SUSPENSIONS_MIN, SUSPENSIONS_MAX)) / 2;
    fr_dx_susp = (volatile uint8_t) (fr_dx_susp + map(filter_buffer(buf[obufn] + 4, ADC_BUFFER_SIZE, ADC_CHANNELS), ADC_MIN, ADC_MAX, SUSPENSIONS_MIN, SUSPENSIONS_MAX)) / 2;

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

    // APPS plausibility
    if (abs(tps1_percentage, tps2_percentage) > APPS_PLAUS_RANGE)
      plaus1 = false;
    else
      plaus1 = true;

    // check APPS + brake plausibility (BSPD)
    if (tps1_percentage > 5 && brake_percentage > 25) // ACCELERATOR + BRAKE plausibility
      plaus2 = false;
    else if (!brake_percentage)
      plaus2 = true;
    
  #elif defined(_RETRO_)
    rt_sx_susp = (volatile uint8_t) (rt_sx_susp + map(filter_buffer(buf[obufn] + 2, ADC_BUFFER_SIZE, ADC_CHANNELS), ADC_MIN, ADC_MAX, SUSPENSIONS_MIN, SUSPENSIONS_MAX)) / 2;
    rt_dx_susp = (volatile uint8_t) (rt_dx_susp + map(filter_buffer(buf[obufn] + 3, ADC_BUFFER_SIZE, ADC_CHANNELS), ADC_MIN, ADC_MAX, SUSPENSIONS_MIN, SUSPENSIONS_MAX)) / 2;


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

#if defined(_FRONTAL_)

void fr_sx_pulse() {
  fr_sx_prev = fr_sx_curr;
  fr_sx_curr = micros();
}

void fr_dx_pulse() {
  fr_dx_prev = fr_dx_curr;
  fr_dx_curr = micros();
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

#endif

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

void model_enable_calibrations() {
  calibrate = true;
}
void model_stop_calibrations() {
  calibrate = false;
}

void CAN_pack_model_data(CAN_FRAME* frame) {
  #if defined(_RETRO_)
    if (!tcs_online)
      return;
  #endif

  switch (frame -> id) {
  #if defined(_FRONTAL_)
    case CAN_PEDALS_ID:
      frame -> length = 3;
      frame -> data.byte[0] = tps1_percentage;
      frame -> data.byte[1] = tps2_percentage;
      frame -> data.byte[2] = brake_percentage;
      frame -> data.byte[3] = (plaus1 ? (0xFF << 4) : 0) | (plaus2 ? 0xFF : 0);
      break;
    case CAN_FRONTAL_ID:
      frame -> length = 6;
      frame -> data.s0 = get_fr_dx_rpm();
      frame -> data.s1 = get_fr_sx_rpm();
      frame -> data.byte[4] = fr_sx_susp;
      frame -> data.byte[5] = fr_dx_susp;
      break;
  #elif defined(_RETRO_)
    case CAN_RETRO_ID:
      frame -> length = 8;
      frame -> data.s0 = get_rt_dx_rpm();
      frame -> data.s1 = get_rt_sx_rpm();
      frame -> data.byte[4] = acc_x_value;
      frame -> data.byte[5] = acc_y_value;
      frame -> data.byte[6] = rt_sx_susp;
      frame -> data.byte[7] = rt_dx_susp;
      break;
  #endif
    default: {}
  }
}

void CAN_unpack_model_data(CAN_FRAME* frame) {
  switch (frame -> id) {
  #if defined(_RETRO_)
    case CAN_PEDALS_ID:
      tps1_percentage = frame -> data.byte[0];
      tps2_percentage = frame -> data.byte[1];
      brake_percentage = frame -> data.byte[2];
      plaus1 = 0xF0 & frame -> data.byte[3];
      plaus2 = 0x0F & frame -> data.byte[3];
      break;
    case CAN_FRONTAL_ID:
      fr_dx_rpm = frame -> data.s0;
      fr_sx_rpm = frame -> data.s1;
      fr_sx_susp = frame -> data.byte[4];
      fr_dx_susp = frame -> data.byte[5];
      break;
  #endif
    default: {}
  }
}

#if defined(_RETRO_) // log data telemetry via radio by RETRO SCU
void SPI_send_string(String str) {

}

void RADIO_send_model() {
  StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
  String    log = "";

  JsonObject&   root = jsonBuffer.createObject();

  JsonObject& pedals = root.createNestedObject("pedals");
  JsonObject& suspensions = root.createNestedObject("suspensions");
  JsonObject& wheels = root.createNestedObject("wheels");
  JsonObject& accelerometers = root.createNestedObject("accelerometers");

  pedals["tps1"] = tps1_percentage;
  pedals["tps2"] = tps1_percentage;
  pedals["brake"] = brake_percentage;
  pedals["tps1_min"] = tps1_low;
  pedals["tps1_max"] = tps1_max;
  pedals["tps2_min"] = tps2_low;
  pedals["tps2_max"] = tps2_max;
  pedals["brake_min"] = brake_low;
  pedals["brake_max"] = brake_max;
  pedals["plaus1"] = plaus1;
  pedals["plaus2"] = plaus2;
  suspensions["front_sx"] = fr_sx_susp;
  suspensions["front_dx"] = fr_dx_susp;
  suspensions["retro_sx"] = rt_sx_susp;
  suspensions["retro_dx"] = rt_dx_susp;
  wheels["front_sx"] = fr_sx_rpm;
  wheels["front_dx"] = fr_dx_rpm;
  wheels["retro_sx"] = rt_sx_rpm;
  wheels["retro_dx"] = rt_dx_susp;
  accelerometers["acc_x"] = acc_x_value;
  accelerometers["acc_y"] = acc_y_value;

  root.printTo(Serial);
  Serial.print("\r\n");
  //root.printTo(log);
  //SPI_send_string(log);
}
#endif
