#ifndef _SENSORS_PINOUT_
#define _SENSORS_PINOUT_

#include <Arduino.h>
#include "common.h"

#if defined(_FRONTAL_)
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

#elif defined(_RETRO_)

	#define RT_SX_PW_PIN		      	36
	#define RT_DX_PW_PIN		      	38

	#define ACC_X_PIN			        A0
	#define ACC_X_ADC_CHAN_NUM    		ADC_CHER_CH7   // AD7

	#define ACC_Z_PIN			        A1
	#define ACC_Z_ADC_CHAN_NUM    		ADC_CHER_CH6   // AD6

	#define RT_SX_SUSP_PIN		    	A2
	#define RT_SX_ADC_CHAN_NUM    		ADC_CHER_CH5   // AD5

	#define RT_DX_SUSP_PIN		    	A3
	#define RT_DX_ADC_CHAN_NUM    		ADC_CHER_CH4  // AD4

	#define RADIO_CSN_PIN				7
	#define RADIO_CE_PIN				8

#endif

#endif /* _SENSORS_PINOUT_ */
