#ifndef _SENSORS_PINOUT_
#define _SENSORS_PINOUT_

#include <Arduino.h>

#define TPS1_PIN			        A0
#define TPS1_ADC_CHAN_NUM     		ADC_CHER_CH7   // AD7

#define TPS2_PIN			        A1
#define TPS2_ADC_CHAN_NUM     		ADC_CHER_CH6   // AD6

#define BRAKE_PIN			        A2
#define BRAKE_ADC_CHAN_NUM    		ADC_CHER_CH5   // AD5

#define FR_SX_PW_PIN		      	36
#define FR_DX_PW_PIN		      	38

#define FR_SX_SUSP_PIN        		A3
#define FR_SX_ADC_CHAN_NUM    		ADC_CHER_CH4   // AD4

#define FR_DX_SUSP_PIN        		A4
#define FR_DX_ADC_CHAN_NUM    		ADC_CHER_CH3   // AD3

#define RT_SX_PW_PIN		      	40
#define RT_DX_PW_PIN		      	42

#define ACC_X_PIN			        A5
#define ACC_X_ADC_CHAN_NUM    		ADC_CHER_CH2   // AD2

#define ACC_Y_PIN			        A6
#define ACC_Y_ADC_CHAN_NUM    		ADC_CHER_CH1   // AD1

#define RT_SX_SUSP_PIN		    	A7
#define RT_SX_ADC_CHAN_NUM    		ADC_CHER_CH0   // AD0

#define RT_DX_SUSP_PIN		    	A8
#define RT_DX_ADC_CHAN_NUM    		ADC_CHER_CH10  // AD10

#endif /* _SENSORS_PINOUT_ */
