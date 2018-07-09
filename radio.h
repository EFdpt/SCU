/** 
 *  @file           radio.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          Radio header file
 */

#ifndef _RADIO_H_
#define _RADIO_H_

#include "common.h"

/**
 *  @page Radio_page Real Time Telemetry System
 *  A real time telemetry system was implemented by a radio bridge.
 *  \f$SCU_{REAR}\f$ is connected to a radio transmission module, which sends 
 *  the telemetry data to a receiving radio module placed at the boxes.
 *  The data is serialized in JSON format and encrypted using AES with a 192bit 
 *  key (in CTR mode).
 *  The ciphertext thus obtained is coded in base64 and sent via radio.
 *  In reception, the text is decoded and decrypted and sent via serial port to 
 *  a computer.
 *  The data thus obtained are deserialized by a multiplatform application 
 *  developed in JavaFX, which has the ability to graph the values of the 
 *  various sensors in real time and to save them in log files to be analyzed
 *  later.
 */

/**
 *  @defgroup Radio_module Radio module
 *   @{
 */

#if defined(_RETRO_)

    /**
     *  @var volatile bool radio_transmit;
     *  @brief Radio transmit enable flag
     */
    extern volatile bool radio_transmit;

    /**
     *  @brief      Initialize radio.
     *  
     *  @author     Arella Matteo <br/>
     *                  (mail: arella.1646983@studenti.uniroma1.it)
     */
    void radio_init();

    /**
     *  @brief      Send vehicle model over radio.
     *  
     *  @author     Arella Matteo <br/>
     *                  (mail: arella.1646983@studenti.uniroma1.it)
     */
    void radio_send_model();

#endif

/**
 *  @}
 */

#endif /* _RADIO_H_ */