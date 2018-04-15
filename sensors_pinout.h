#ifndef _SENSORS_PINOUT_
#define _SENSORS_PINOUT_

#define TPS1_PIN			        A0
#define TPS1_ADC_CHAN_NUM     7   // AD7

#define TPS2_PIN			        A1
#define TPS2_ADC_CHAN_NUM     6   // AD6

#define BRAKE_PIN			        A2
#define BRAKE_ADC_CHAN_NUM    5   // AD5

#define FR_SX_PW_PIN		      36
#define FR_DX_PW_PIN		      38

#define FR_SX_SUSP_PIN        A3
#define FR_SX_ADC_CHAN_NUM    4   // AD4

#define FR_DX_SUSP_PIN        A4
#define FR_DX_ADC_CHAN_NUM    3   // AD3

#define RT_SX_PW_PIN		      40
#define RT_DX_PW_PIN		      42

#define ACC_X_PIN			        A5
#define ACC_X_ADC_CHAN_NUM    2   // AD2

#define ACC_Y_PIN			        A6
#define ACC_Y_ADC_CHAN_NUM    1   // AD1

#define RT_SX_SUSP_PIN		    A7
#define RT_SX_ADC_CHAN_NUM    0   // AD0

#define RT_DX_SUSP_PIN		    A8
#define RT_DX_ADC_CHAN_NUM    10   // AD10

#define ADC_CHANNEL_ENABLE(chan_num)    \
          ADC_CHER_CH##chan_num

#endif /* _SENSORS_PINOUT_ */
