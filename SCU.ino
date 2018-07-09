/** 
 *  @file           SCU.ino
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          Main module file
 */

#include "common.h"
#include "states.h"
#include "nmt.h"
#include "radio.h"

/**
 *  @mainpage FastChargeSAE SCU firmware
 *
 *  CAN network arises from the need to digitize all those signals necessary 
 *  for the operation of the vehicle.
 *
 *  Two Arduino Due prototyping boards have been adopted for signal digitalization:
 *  first one located at the front of the vehicle, reserved for the acquisition of 
 *  pedals, frontal suspensions and frontal wheel groups, the second one placed 
 *  an the rear of the vehicle, to acquire rear suspensions, rear wheels and 
 *  accelerometers.
 *
 *  Sensor acquisition boards will now be named SCU (Sensors Control Unit) and 
 *  \f$SCU_{FRONTAL}\f$, \f$SCU_{REAR}\f$ respectively for SCU located at the 
 *  front and at the back of the vehicle.
 *
 *  Each board performs mainly two actions:
 *  -   Sensor acquisition
 *  -   Data transmission over CAN servizi network and over radio (for real time
 *      telemetry)
 *
 *  A protocol layer above the data link layer (CAN protocol) is implemented 
 *  inspired by the CANOpen communication protocol; each node is addressable at 
 *  the network level using a specific and unique ID for every node.
 *
 *  The firmware for each node is selectable during the precompilation of the
 *  code from the directives present in @ref SCU_firmware_selection.
 */

/**
 *  @defgroup Main_group_module Main module
 *   @{
 */

/**
 *  @brief      This function perform basic board setup.
 *              Upon power-up SCU (CANopen slave node) goes into initialization. 
 *              It initializes the entire application, CAN/CANopen interfaces 
 *              and communication. At the end of the initialization the node 
 *              tries to transmit boot-up message. As soon as it is transmitted 
 *              successfully, the node switches to Pre-operational state.
 *              
 *  @author     Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 */
void setup() {
    #if defined(_FRONTAL_)
    setNodeId(SCU_FRONTAL_NODE_ID);
    #elif defined(_RETRO_)
    setNodeId(SCU_REAR_NODE_ID);
    #endif
    initialisation();
    slaveSendBootUp();  
    preOperational();
}


/**
 *  @brief      This function is called into endless while main loop.
 *              It takes care of sending data through radio, if enabled.
 * 
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 */
__attribute__((__inline__))
void loop() {
    if (radio_transmit) {
        radio_transmit = false;
        radio_send_model();
    } else {
        /* Go in Wait For Interrupt mode for reducing power consumption */
        __WFI();
    }
}

/**
 *  @}
 */
