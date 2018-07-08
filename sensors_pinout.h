/** 
 *  @file           sensors_pinout.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          Board pinout module header
 */

#ifndef _SENSORS_PINOUT_
#define _SENSORS_PINOUT_

#include <Arduino.h>
#include "common.h"

/** @addtogroup Board_model_group
 *  
 *  @{
 */

/**
 *  @defgroup Board_pinout_group Board pinout
 *   @{
 *   Board pinout for each sensor, CAN port and radio.
 */

/**
 *  @def CAN_PORT
 *  @brief Pin on board dedicated to CAN port
 */
#define CAN_PORT            Can0

#if defined(_FRONTAL_)
	/**
	 *  @def TPS1_PIN
	 *  @brief Pin on board dedicated to first APPS (tps1)
	 */
	#define TPS1_PIN			        A0

	/**
	 *  @def TPS1_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to tps1 signal (AD7)
	 */
	#define TPS1_ADC_CHAN_NUM     		ADC_CHER_CH7

	/**
	 *  @def TPS2_PIN
	 *  @brief Pin on board dedicated to second APPS (tps2)
	 */
	#define TPS2_PIN			        A1

	/**
	 *  @def TPS2_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to tps2 signal (AD6)
	 */
	#define TPS2_ADC_CHAN_NUM     		ADC_CHER_CH6

	/**
	 *  @def BRAKE_PIN
	 *  @brief Pin on board dedicated to brake pedal position sensor
	 */
	#define BRAKE_PIN			        A2

	/**
	 *  @def BRAKE_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to brake signal (AD5)
	 */
	#define BRAKE_ADC_CHAN_NUM    		ADC_CHER_CH5

	/**
	 *  @def FR_SX_SUSP_PIN
	 *  @brief Pin on board dedicated to frontal left suspension sensor
	 */
	#define FR_SX_SUSP_PIN        		A3

	/**
	 *  @def FR_SX_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to frontal
	 *         left suspension signal (AD4)
	 */
	#define FR_SX_ADC_CHAN_NUM    		ADC_CHER_CH4

	/**
	 *  @def FR_DX_SUSP_PIN
	 *  @brief Pin on board dedicated to frontal right suspension sensor
	 */
	#define FR_DX_SUSP_PIN        		A4

	/**
	 *  @def FR_DX_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to frontal
	 *         right suspension signal (AD3)
	 */
	#define FR_DX_ADC_CHAN_NUM    		ADC_CHER_CH3

	/**
	 *  @def FR_SX_PW_PIN
	 *  @brief Pin on board dedicated to frontal left phonic wheel encoder
	 */
	#define FR_SX_PW_PIN		      	36

	/**
	 *  @def FR_DX_PW_PIN
	 *  @brief Pin on board dedicated to frontal right phonic wheel encoder
	 */
	#define FR_DX_PW_PIN		      	38

#elif defined(_RETRO_)

	/**
	 *  @def RT_SX_PW_PIN
	 *  @brief Pin on board dedicated to retro left phonic wheel encoder
	 */
	#define RT_SX_PW_PIN		      	36

	/**
	 *  @def RT_DX_PW_PIN
	 *  @brief Pin on board dedicated to retro right phonic wheel encoder
	 */
	#define RT_DX_PW_PIN		      	38

	/**
	 *  @def ACC_X_PIN
	 *  @brief Pin on board dedicated to accelerometer X sensor
	 */
	#define ACC_X_PIN			        A0

	/**
	 *  @def ACC_X_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to accelerometer X signal (AD7)
	 */
	#define ACC_X_ADC_CHAN_NUM    		ADC_CHER_CH7

	/**
	 *  @def ACC_Z_PIN
	 *  @brief Pin on board dedicated to accelerometer Z sensor
	 */
	#define ACC_Z_PIN			        A1

	/**
	 *  @def ACC_Z_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to accelerometer Z signal (AD6)
	 */
	#define ACC_Z_ADC_CHAN_NUM    		ADC_CHER_CH6

	/**
	 *  @def RT_SX_SUSP_PIN
	 *  @brief Pin on board dedicated to retro left suspension sensor
	 */
	#define RT_SX_SUSP_PIN		    	A2

	/**
	 *  @def RT_SX_SUSP_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to retro
	 *         left suspension signal (AD5)
	 */
	#define RT_SX_SUSP_ADC_CHAN_NUM    	ADC_CHER_CH5

	/**
	 *  @def RT_DX_SUSP_PIN
	 *  @brief Pin on board dedicated to right retro suspension sensor
	 */
	#define RT_DX_SUSP_PIN		    	A3

	/**
	 *  @def RT_DX_SUSP_ADC_CHAN_NUM
	 *  @brief GPIO pin on the Atmel SAM3X8E processor corresponding to retro
	 *         right suspension signal (AD4)
	 */
	#define RT_DX_SUSP_ADC_CHAN_NUM    	ADC_CHER_CH4

	/**
	 *  @def RADIO_CSN_PIN
	 *  @brief SPI CSN pin for radio module
	 */
	#define RADIO_CSN_PIN				7

	/**
	 *  @def RADIO_CE_PIN
	 *  @brief SPI CE pin for radio module
	 */
	#define RADIO_CE_PIN				8

#endif

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* _SENSORS_PINOUT_ */
