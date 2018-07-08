/** 
 *  @file           model.cpp
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          Board model implementation file
 */

#include "model.h"
#include "filter.h"

/**
 *  @addtogroup Board_model_group
 *   @{
 */

#undef HID_ENABLED

/**
 *  @def ADC_BUFFER_SIZE
 *  @brief Size (bytes) of buffer for store each ADC channel data with DMA
 */
#define ADC_BUFFER_SIZE                 128

/**
 *  @def        BUFFERS
 *  @brief      Number of DMA buffers
 *  @warning    Must be a power of two
 */
#define BUFFERS                         4

/**
 *  @def ADC_MIN
 *  @brief ADC lower bound value
 */
#define ADC_MIN                         0

/**
 *  @def ADC_MAX
 *  @brief ADC upper bound value
 */
#define ADC_MAX                         4095

/**
 * @def     TPS1_UPPER_BOUND
 * @brief   First APPS max output voltage (2V)
 */
#define TPS1_UPPER_BOUND            2482

/**
 * @def     TPS1_LOWER_BOUND
 * @brief   First APPS min output voltage (0.8V)
 */
#define TPS1_LOWER_BOUND            993

/**
 * @def     TPS2_UPPER_BOUND
 * @brief   Second APPS max output voltage (1V)
 */
#define TPS2_UPPER_BOUND            1241

/**
 * @def     TPS2_LOWER_BOUND
 * @brief   Second APPS min output voltage (0.4V)
 */
#define TPS2_LOWER_BOUND            497

/**
 * @def     BRAKE_UPPER_BOUND
 * @brief   Brake sensor max output voltage (TODO: check Voutmax)
 */
#define BRAKE_UPPER_BOUND           0

/**
 * @def     BRAKE_LOWER_BOUND
 * @brief   Brake sensor min output voltage (TODO: check Voutmin)
 */
#define BRAKE_LOWER_BOUND           ADC_MAX

/**
 *  @def SUSPENSIONS_MIN
 *  @brief Minimum suspension stroke [\f$mm\f$]
 */
#define SUSPENSIONS_MIN                 0

/**
 *  @def SUSPENSIONS_ADC_MAX
 *  @brief Maximum suspension sensor \f$V_{OUT}\f$
 */
#define SUSPENSIONS_ADC_MAX             ADC_MAX

/**
 * @def SUSP_STROKE_NORMALIZE
 * @brief Suspension stroke voltage normalizer
 */
#define SUSP_STROKE_NORMALIZE (SUSP_STROKE_EXTENSION / SUSPENSIONS_ADC_MAX)

/**
 *  @def COGS_NUMBER
 *  @brief Number of phonic wheel's cogs
 */
#define COGS_NUMBER                     30.0d

/**
 *  @def NORMALIZE_RPM
 *  @brief Normalize time domain [\f$\mu s\f$]
 */
#define NORMALIZE_RPM                   1000000.0d

/**
 *  @def RPM_MIN
 *  @brief Rpm lower bound under that rpm are forced to zero
 */
#define RPM_MIN                         10

/**
 *  @def ACCELEROMETER_MAX_G
 *  @brief Accelerometer sensor maximum value [\f$m/s^{2}\f$]
 */
#define ACCELEROMETER_MAX_G             5.0d

/**
 * @def ACCELEROMETER_NORMALIZE
 * @brief Accelerometer sensor voltage normalizer
 */
#define ACCELEROMETER_NORMALIZE         2.0d * ACCELEROMETER_MAX_G / ADC_MAX

/**
 *  @def APPS_PLAUS_RANGE
 *  @brief Maximum percentage deviation of pedal travel between two APPS
 */
#define APPS_PLAUS_RANGE                10

/**
 *  @def SCU_FRONTAL_ADC_CHANNELS
 *  @brief Number of ADC channels used in frontal SCU board
 */
#define SCU_FRONTAL_ADC_CHANNELS        5

/**
 *  @def SCU_FRONTAL_ADC_CHANNELS_LIST
 *  @brief List of ADC channels dedicated to each IO port in frontal SCU board
 */
#define SCU_FRONTAL_ADC_CHANNELS_LIST   TPS1_ADC_CHAN_NUM | TPS2_ADC_CHAN_NUM | BRAKE_ADC_CHAN_NUM | FR_SX_ADC_CHAN_NUM | FR_DX_ADC_CHAN_NUM

/**
 *  @def SCU_RETRO_ADC_CHANNELS
 *  @brief Number of ADC channels used in rear SCU board
 */
#define SCU_RETRO_ADC_CHANNELS          4

/**
 *  @def SCU_RETRO_ADC_CHANNELS_LIST
 *  @brief List of ADC channels dedicated to each IO port in retro SCU board
 */
#define SCU_RETRO_ADC_CHANNELS_LIST     ACC_X_ADC_CHAN_NUM | ACC_Z_ADC_CHAN_NUM | RT_SX_ADC_CHAN_NUM | RT_DX_ADC_CHAN_NUM   

/**
 *  @def TPS1_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define TPS1_ADC_OFFSET                 0

/**
 *  @def TPS2_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define TPS2_ADC_OFFSET                 1

/**
 *  @def BRAKE_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define BRAKE_ADC_OFFSET                2

/**
 *  @def FR_SX_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define FR_SX_ADC_OFFSET                3

/**
 *  @def FR_DX_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define FR_DX_ADC_OFFSET                4

/**
 *  @def ACC_X_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define ACC_X_ADC_OFFSET                0

/**
 *  @def ACC_Z_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define ACC_Z_ADC_OFFSET                1

/**
 *  @def RT_SX_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define RT_SX_ADC_OFFSET                2

/**
 *  @def RT_DX_ADC_OFFSET
 *  @brief Offset from DMA buffer
 */
#define RT_DX_ADC_OFFSET                3

#ifndef __DOXYGEN__
    #if defined(_FRONTAL_)
        #define ADC_CHANNELS            SCU_FRONTAL_ADC_CHANNELS
        #define ADC_CHANNELS_LIST       SCU_FRONTAL_ADC_CHANNELS_LIST
            
      
    #elif defined(_RETRO_)
        #define ADC_CHANNELS            SCU_RETRO_ADC_CHANNELS
        #define ADC_CHANNELS_LIST       SCU_RETRO_ADC_CHANNELS_LIST
    #endif
#endif

/**
 *  @def BUFFER_LENGTH
 *  @brief Length, in bytes, of each DMA buffer
 */
#define BUFFER_LENGTH       ADC_BUFFER_SIZE * ADC_CHANNELS

/**
 *  @var    volatile int bufn;
 *  @brief  DMA buffer pointer
 */
volatile int        bufn;

/**
 *  @var    volatile int obufn;
 *  @brief  DMA buffer pointer
 */
volatile int        obufn;

/**
 * @var     volatile uint16_t   buf[#BUFFERS][#BUFFER_LENGTH];
 * @brief   DMA buffers: #BUFFERS number of buffers each of #BUFFER_LENGTH size;
 *          DMA is configured in cyclic mode: after one of #BUFFERS is filled then 
 *          DMA transfer head moves to next buffer in circular indexing.
 */
volatile uint16_t   buf[BUFFERS][BUFFER_LENGTH];

#if defined(_FRONTAL_)

    /**
     * @var     volatile uint16_t tps1_value;
     * @brief   First APPS value retrieved directly by analog tps1 signal (#TPS1_PIN)
     *          and filtered after DMA buffer is filled entirely
     */
    volatile uint16_t   tps1_value = 0;

    /**
     * @var     volatile uint16_t tps2_value;
     * @brief   Second APPS value retrieved directly by analog tps2 signal (#TPS2_PIN)
     *          and filtered after DMA buffer is filled entirely
     */
    volatile uint16_t   tps2_value = 0;

    /**
     * @var     volatile uint16_t brake_value;
     * @brief   Brake pedal position sensor value retrieved directly by analog brake
     *          signal (#BRAKE_PIN) and filtered after DMA buffer is filled entirely
     */
    volatile uint16_t   brake_value = 0;

    /**
     *  @var    volatile uint16_t tps1_max;
     *  @brief  First APPS max output voltage (equals to #TPS1_UPPER_BOUND)
     */
    volatile uint16_t   tps1_max    = TPS1_UPPER_BOUND;

    /**
     *  @var    volatile uint16_t tps1_min;
     *  @brief  First APPS min output voltage (equals to #TPS1_LOWER_BOUND)
     */
    volatile uint16_t   tps1_min    = TPS1_LOWER_BOUND;

    /**
     *  @var    volatile uint16_t tps2_max;
     *  @brief  Second APPS max output voltage (equals to #TPS2_UPPER_BOUND)
     */
    volatile uint16_t   tps2_max    = TPS2_UPPER_BOUND;

    /**
     *  @var    volatile uint16_t tps2_min;
     *  @brief  Second APPS min output voltage (equals to #TPS2_LOWER_BOUND)
     */
    volatile uint16_t   tps2_min    = TPS2_LOWER_BOUND;

    /**
     *  @var    volatile uint16_t brake_max;
     *  @brief  Brake sensor max output voltage (equals to #BRAKE_UPPER_BOUND)
     */
    volatile uint16_t   brake_max   = BRAKE_UPPER_BOUND;

    /**
     *  @var    volatile uint16_t brake_min;
     *  @brief  Brake sensor min output voltage (equals to #BRAKE_LOWER_BOUND)
     */
    volatile uint16_t   brake_min   = BRAKE_LOWER_BOUND;

    #if 0
    volatile bool     calibrate = true;   // calibrate pedals
    #endif

#endif

/**
 * @var     volatile uint8_t tps1_percentage;
 * @brief   First APPS percentage value retrieved by tps1 signal (#TPS1_PIN)
 */
volatile uint8_t  tps1_percentage = 0;

/**
 * @var     volatile uint8_t tps2_percentage;
 * @brief   Second APPS percentage value retrieved by tps2 signal (#TPS2_PIN)
 */
volatile uint8_t  tps2_percentage = 0;

/**
 * @var     volatile uint8_t brake_percentage
 * @brief   Brake pedal position sensor percentage value retrieved by brake
 *          signal (#BRAKE_PIN)
 */
volatile uint8_t  brake_percentage = 0;

/**
 * @var     volatile bool apps_plausibility
 * @brief   APPS plausibility status
 */
volatile bool     apps_plausibility = true;

/**
 * @var     volatile bool brake_plausibility
 * @brief   Brake plausibility status
 */
volatile bool     brake_plausibility = true;

/**
 *  @var volatile uint8_t  fr_sx_susp;
 *  @brief Frontal left suspension stroke [\f$mm\f$]
 */
volatile uint8_t fr_sx_susp;

/**
 *  @var volatile uint8_t  fr_dx_susp;
 *  @brief Frontal right suspension stroke [\f$mm\f$]
 */
volatile uint8_t fr_dx_susp;

/**
 *  @var volatile uint16_t  fr_sx_rpm;
 *  @brief Frontal left wheel velocity [\f$rpm\f$]
 */
volatile uint16_t fr_sx_rpm = 0;

/**
 *  @var volatile uint16_t  fr_dx_rpm;
 *  @brief Frontal right wheel velocity [\f$rpm\f$]
 */
volatile uint16_t fr_dx_rpm = 0;

#if defined(_FRONTAL_)

    /**
     *  @var volatile unsigned long fr_sx_prev;
     *  @brief Frontal left wheel encoder previous timestamp
     */
    volatile unsigned long fr_sx_prev;

    /**
     *  @var volatile unsigned long fr_sx_curr;
     *  @brief Frontal left wheel encoder current timestamp
     */
    volatile unsigned long fr_sx_curr;

    /**
     *  @var volatile unsigned long fr_dx_prev;
     *  @brief Frontal right wheel encoder previous timestamp
     */
    volatile unsigned long fr_dx_prev;

    /**
     *  @var volatile unsigned long fr_dx_curr;
     *  @brief Frontal right wheel encoder current timestamp
     */
    volatile unsigned long fr_dx_curr;

#elif defined(_RETRO_)

    /**
     *  @var volatile unsigned long rt_sx_prev;
     *  @brief Rear left wheel encoder previous timestamp
     */
    volatile unsigned long rt_sx_prev;

    /**
     *  @var volatile unsigned long rt_sx_prev;
     *  @brief Rear left wheel encoder current timestamp
     */
    volatile unsigned long rt_sx_curr;

    /**
     *  @var volatile unsigned long rt_dx_prev;
     *  @brief Rear right wheel encoder previous timestamp
     */
    volatile unsigned long rt_dx_prev;

    /**
     *  @var volatile unsigned long rt_dx_prev;
     *  @brief Rear right wheel encoder current timestamp
     */
    volatile unsigned long rt_dx_curr;
    
    /**
     *  @var volatile uint8_t acc_x_value;
     *  @brief Accelerometer X value [\f$m/s^{2}\f$]
     */
    volatile uint8_t acc_x_value = 0;

    /**
     *  @var volatile uint8_t acc_z_value;
     *  @brief Accelerometer Z value [\f$m/s^{2}\f$]
     */
    volatile uint8_t acc_z_value = 0;
    
    /**
     *  @var volatile uint8_t  rt_sx_susp;
     *  @brief Rear left suspension stroke [\f$mm\f$]
     */
    volatile uint8_t rt_sx_susp = 0;

    /**
     *  @var volatile uint8_t  rt_dx_susp;
     *  @brief Rear right suspension stroke [\f$mm\f$]
     */
    volatile uint8_t rt_dx_susp = 0;

    /**
     *  @var volatile uint16_t  rt_sx_rpm;
     *  @brief Rear left wheel velocity [\f$rpm\f$]
     */
    volatile uint16_t rt_sx_rpm = 0;

    /**
     *  @var volatile uint16_t  rt_dx_rpm;
     *  @brief Rear right wheel velocity [\f$rpm\f$]
     */
    volatile uint16_t rt_dx_rpm = 0;

#endif

/**
 * @var     volatile uint16_t   buf[#BUFFERS][#BUFFER_LENGTH];
 * @brief   DMA buffers: #BUFFERS number of buffers each of #BUFFER_LENGTH size;
 *          DMA is configured in cyclic mode: after one of #BUFFERS is filled then 
 *          DMA transfer head moves to next buffer in circular indexing.
 */
volatile uint16_t buf[BUFFERS][BUFFER_LENGTH];

#if defined(_FRONTAL_)

/**
 *  @brief      EXTI IRQ handler.
 *              External interrupt handler executed when frontal left wheel
 *              encoder finds a hole into phonic wheel.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
void fr_sx_pulse() {
  fr_sx_prev = fr_sx_curr;
  fr_sx_curr = micros();
}

/**
 *  @brief      EXTI IRQ handler.
 *              External interrupt handler executed when frontal right wheel
 *              encoder finds a hole into phonic wheel.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
void fr_dx_pulse() {
  fr_dx_prev = fr_dx_curr;
  fr_dx_curr = micros();
}

/**
 *  @brief      If rpm value is lower than #RPM_MIN, output is forced to zero.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Frontal left wheel rpm
 */
__attribute__((__inline__))
volatile uint16_t get_fr_sx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (fr_sx_curr - fr_sx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

/**
 *  @brief      If rpm value is lower than #RPM_MIN, output is forced to zero.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Frontal right wheel rpm
 */
__attribute__((__inline__))
volatile uint16_t get_fr_dx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (fr_dx_curr - fr_dx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

#elif defined(_RETRO_)

/**
 *  @brief      EXTI IRQ handler.
 *              External interrupt handler executed when rear left wheel
 *              encoder finds a hole into phonic wheel.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
void rt_sx_pulse() {
  rt_sx_prev = rt_sx_curr;
  rt_sx_curr = micros();
}

/**
 *  @brief      EXTI IRQ handler.
 *              External interrupt handler executed when rear right wheel
 *              encoder finds a hole into phonic wheel.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
void rt_dx_pulse() {
  rt_dx_prev = rt_dx_curr;
  rt_dx_curr = micros();
}

// frontal rpm values getted from CAN frame
__attribute__((__inline__))
volatile uint16_t get_fr_sx_rpm() {
  return fr_sx_rpm;
}

__attribute__((__inline__))
volatile uint16_t get_fr_dx_rpm() {
  return fr_dx_rpm;
}

/**
 *  @brief      If rpm value is lower than #RPM_MIN, output is forced to zero.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Rear left wheel rpm
 */
__attribute__((__inline__))
volatile uint16_t get_rt_sx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (rt_sx_curr - rt_sx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

/**
 *  @brief      If rpm value is lower than #RPM_MIN, output is forced to zero.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Rear right wheel rpm
 */
__attribute__((__inline__))
volatile uint16_t get_rt_dx_rpm() {
  volatile double rpm;
  return ((rpm = (60.0 / COGS_NUMBER) * (NORMALIZE_RPM / (double) (rt_dx_curr - rt_dx_prev))) < RPM_MIN) ?
    0 : (volatile uint16_t) rpm;
}

#endif

/**
 *  @brief      This function filters ADC acquisitions;
 *              Each ADC buffer data is filtered and an average is done with 
 *                  previous values.
 *                  
 *              Frontal SCU:
 *              -   Each pedal filtered value is mapped to a percentage value;
 *              -   APPS plausibility and brake plausibility are checked;
 *              -   Suspension values filtering.
 *
 *              Rear SCU:
 *              -   Suspensions' values filtering;
 *              -   Accelerometers' values filtering.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
static inline void filter_data() {
  #if defined(_FRONTAL_)
    tps1_value = (tps1_value + filter_buffer(buf[obufn] + TPS1_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;
    tps2_value = (tps2_value + filter_buffer(buf[obufn] + TPS2_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;
    brake_value = (brake_value + filter_buffer(buf[obufn] + BRAKE_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS)) / 2;

    fr_sx_susp = (volatile uint8_t) (fr_sx_susp + (SUSP_STROKE_NORMALIZE * filter_buffer(buf[obufn] + FR_SX_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;
    fr_dx_susp = (volatile uint8_t) (fr_dx_susp + (SUSP_STROKE_NORMALIZE * filter_buffer(buf[obufn] + FR_DX_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;

    #if 0
    if (calibrate) {
      if (tps1_value < tps1_low) tps1_low = tps1_value;
      if (tps1_value > tps1_max) tps1_max = tps1_value;
      if (tps2_value < tps2_low) tps2_low = tps2_value;
      if (tps2_value > tps2_max) tps2_max = tps2_value;
      if (brake_value < brake_low) brake_low = brake_value;
      if (brake_value > brake_max) brake_max = brake_value;
    }
    #endif

    if (tps1_value < tps1_min) tps1_value = tps1_min;
    if (tps1_value > tps1_max) tps1_value = tps1_max;
    if (tps2_value < tps2_min) tps2_value = tps2_min;
    if (tps2_value > tps2_max) tps2_value = tps2_max;
    if (brake_value < brake_min) brake_value = brake_min;
    if (brake_value > brake_max) brake_value = brake_max;

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
  
    acc_x_value = (volatile uint8_t) (acc_x_value + (ACCELEROMETER_NORMALIZE * filter_buffer(buf[obufn] + ACC_X_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;
    acc_z_value = (volatile uint8_t) (acc_z_value + (ACCELEROMETER_NORMALIZE * filter_buffer(buf[obufn] + ACC_Z_ADC_OFFSET, ADC_BUFFER_SIZE, ADC_CHANNELS))) / 2;
  #endif
}

/**
 *  @brief      ADC IRQ handler.
 *              When ADC buffer is filled DMA pointer is linked to next buffer
 *              available.
 *              Then acquired data are filtered.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
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

/**
 *  @brief      This function initializes hardware board.
 *              
 *              ADC peripheral is initialized with ADC_FREQ_MAX clock and with
 *              12bits of resolution.
 *
 *              ADC peripheral is then configured in free running mode for continuous
 *              acquisitions.
 *
 *              ADC channels are enabled according to #SCU_FRONTAL_ADC_CHANNELS_LIST
 *              or #SCU_RETRO_ADC_CHANNELS_LIST.
 *
 *              ADC End of Receive Buffer Interrupt is enabled for triggering
 *              interrupt when DMA has filled entire buffer.
 *
 *              Then EXTI are enabled for triggering interrupt by wheel encoders.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
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

#if 0
#if defined(_FRONTAL_)
__attribute__((__inline__)) void model_enable_calibrations() {
  calibrate = true;
}
__attribute__((__inline__)) void model_stop_calibrations() {
  calibrate = false;
}
#endif
#endif

/**
 *  @}
 */