/** 
 *  @file           model.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          Board model header file
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include "sensors_pinout.h"
#include "common.h"

#if !defined(_FRONTAL_) && !defined(_RETRO_)
  #error "No board specified"
#endif

/**
 *  @page Model_page Board model
 *
 *  SCUs are based on an Atmel SAM3X8E board with an ARM Cortex-M3 microprocessor.
 *
 *  Analog input signals managed from SCUs are:<br/>
 *      | NODE                | Signal             |
 *      | :-----------------: | :----------------: |
 *      | \f$SCU_{Frontal}\f$ | First APPS         |
 *      | ^                   | Second APPS        |
 *      | ^                   | Brake              |
 *      | ^                   | Right phonic wheel |
 *      | ^                   | Left phonic wheel  |
 *      | ^                   | Right suspension   |
 *      | ^                   | Left suspension    |
 *      | \f$SCU_{Rear}\f$    | Accel. X           |
 *      | ^                   | Accel. Z           |
 *      | ^                   | Right suspension   |
 *      | ^                   | Left suspension    |
 *      | ^                   | Right phonic wheel |
 *      | ^                   | Left phonic wheel  |
 *
 *  Board pinout is described in @ref Board_model_group.
 *
 *  Output rpm are returned following this formula:<br/>
 *      \f[
 *          rpm = NORMALIZE\_RPM * (\frac{60}{COGS\_NUMBER\ *\ \Delta_{TIMESTAMP}})
 *      \f]
 *      where \f$NORMALIZE\_RPM\f$ normalizes \f$TIMESTAMP\f$ in seconds.
 *      
 */

/**
 *  @defgroup Board_model_group Board module
 *   @{
 */

/**
 *  @def SUSP_STROKE_EXTENSION
 *  @brief Maximum suspension stroke [\f$mm\f$]
 */
#define SUSP_STROKE_EXTENSION	75.0

/**
 *  @var volatile uint8_t  tps1_percentage;
 *  @brief First APPS percentage value
 */
extern volatile uint8_t  tps1_percentage;

/**
 *  @var volatile uint8_t  tps2_percentage;
 *  @brief Second APPS percentage value
 */
extern volatile uint8_t  tps2_percentage;

/**
 *  @var volatile uint8_t  brake_percentage;
 *  @brief Brake pedal position sensor percentage value
 */
extern volatile uint8_t  brake_percentage;

/**
 *  @var volatile bool  apps_plausibility;
 *  @brief APPS plausibility flag
 */
extern volatile bool     apps_plausibility;

/**
 *  @var volatile uint8_t  brake_plausibility;
 *  @brief Brake plausibility flag
 */
extern volatile bool     brake_plausibility;

/**
 *  @var volatile uint8_t  fr_sx_susp;
 *  @brief Frontal left suspension stroke [\f$mm\f$]
 */
extern volatile uint8_t fr_sx_susp;

/**
 *  @var volatile uint8_t  fr_dx_susp;
 *  @brief Frontal right suspension stroke [\f$mm\f$]
 */
extern volatile uint8_t fr_dx_susp;

/**
 *  @var volatile uint16_t  fr_sx_rpm;
 *  @brief Frontal left wheel velocity [\f$rpm\f$]
 */
extern volatile uint16_t fr_sx_rpm;

/**
 *  @var volatile uint16_t  fr_dx_rpm;
 *  @brief Frontal right wheel velocity [\f$rpm\f$]
 */
extern volatile uint16_t fr_dx_rpm;

/**
 *  @brief      This function returns frontal left wheel velocity [\f$rpm\f$].
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Frontal left wheel rpm
 */
volatile uint16_t get_fr_sx_rpm();

/**
 *  @brief      This function returns frontal right wheel velocity [\f$rpm\f$].
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Frontal right wheel rpm
 */
volatile uint16_t get_fr_dx_rpm();

#if defined(_RETRO_)

/**
 *  @var volatile uint8_t  acc_x_value;
 *  @brief Accelerometer X value [\f$m/s^{2}\f$]
 */
extern volatile uint8_t acc_x_value;

/**
 *  @var volatile uint8_t  acc_z_value;
 *  @brief Accelerometer Z value [\f$m/s^{2}\f$]
 */
extern volatile uint8_t acc_z_value;

/**
 *  @var volatile uint8_t  rt_sx_susp;
 *  @brief Retro left suspension stroke [\f$mm\f$]
 */
extern volatile uint8_t rt_sx_susp;

/**
 *  @var volatile uint8_t  rt_dx_susp;
 *  @brief Retro right suspension stroke [\f$mm\f$]
 */
extern volatile uint8_t rt_dx_susp;

/**
 *  @brief      This function returns retro left wheel velocity [\f$rpm\f$].
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Retro left wheel rpm
 */
volatile uint16_t get_rt_sx_rpm();

/**
 *  @brief      This function returns retro right wheel velocity [\f$rpm\f$].
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *
 *  @returns    Retro right wheel rpm
 */
volatile uint16_t get_rt_dx_rpm();
#endif

/**
 *  @brief      This function initializes hardware board.
 *              
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
void model_init();

#if 0
#if defined(_FRONTAL_)
void model_enable_calibrations();
void model_stop_calibrations();
#endif
#endif

/**
 *  @}
 */

#endif /* _MODEL_H_ */
