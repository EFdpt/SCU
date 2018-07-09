/** 
 *  @file           timer.cpp
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANopen timer implementation file
 */

#include <DueTimer.h>

#include "common.h"
#include "timer.h"
#include "pdo.h"
#include "CO_can.h"

#if defined(_RETRO_)
    #include "radio.h"
#endif

/**
 *  @addtogroup CANopen_timer_module CANopen Timer module
 *   @{
 */

/**
 *  @def SCU_FRONT_FIRST_SLOT
 *  @brief First time slot for \f$SCU_{Frontal}\f$
 */
#define SCU_FRONT_FIRST_SLOT 			0

/**
 *  @def SCU_FRONT_SECOND_SLOT
 *  @brief Second time slot for \f$SCU_{Frontal}\f$
 */
#define SCU_FRONT_SECOND_SLOT 			1

/**
 *  @def SCU_REAR_SLOT
 *  @brief Time slot for \f$SCU_{Rear}\f$
 */
#define SCU_REAR_SLOT 					2

/**
 *  @def TCS_SLOT
 *  @brief Time slot for \f$TCU\f$
 */
#define TCS_SLOT 						3

/**
 *  @def TIME_SLOT_MASK
 *  @brief Time slot mask
 */
#define TIME_SLOT_MASK                  3

/**
 *  @def RADIO_SLOT_MASK
 *  @brief Radio submit slot mask: number of \f$SCU_{Rear}\f$ time slots between
 *         one submit and successive one (#TIME_SLOT_PERIOD * #RADIO_SLOT_MASK * 
 *         num. time slots between previous and current \f$SCU_{Rear}\f$)
 */
#define RADIO_SLOT_MASK                 7 // 3 slot temporali di _RETRO_ (spedisci via radio ogni TIME_SLOT_PERIOD * 4 * 3, con 4 = #slot di attesa per SCU_RETRO)

/**
 *  @var volatile uint8_t t_slot;
 *  @brief Current time slot.
 */
volatile uint8_t		t_slot = SCU_FRONT_FIRST_SLOT;

#if defined(_RETRO_)
/**
 *  @var volatile uint8_t radio_slot;
 *  @brief Current radio time slot.
 */
volatile uint8_t        radio_slot = 0;
#endif

/**
 *  @var DueTimer* timer;
 *  @brief Timer for periodic TPDO submit.
 */
DueTimer* timer;

void TimeDispatch() { // send PDOs periodically
#if defined(_FRONTAL_)
    switch (t_slot) {
		case SCU_FRONT_FIRST_SLOT: {
			Message m = Message_Initializer;
            buildPDO(PDO1tx, &m);
            canSend(&m);
			break;
        }
		case SCU_FRONT_SECOND_SLOT: {
            Message m = Message_Initializer;
            buildPDO(PDO2tx, &m);
            canSend(&m);
			break;
        }
        default: {}
    }
#elif defined(_RETRO_)
    switch (t_slot) {
		case SCU_REAR_SLOT: {
            Message m = Message_Initializer;
            buildPDO(PDO1tx, &m);
            canSend(&m);
            if (!radio_slot)
                radio_transmit = true;
            radio_slot = (radio_slot + 1) & RADIO_SLOT_MASK;
			break;
        }
        default: {}
    }
#endif

    t_slot = (t_slot + 1) & TIME_SLOT_MASK;
}

__attribute__((__inline__)) void timerInit() {
	TIMER.attachInterrupt(TimeDispatch);
}

__attribute__((__inline__)) void timerStart() {
	TIMER.start(TIME_SLOT_PERIOD);
}

__attribute__((__inline__)) void timerStop() {
	TIMER.stop();
}

/**
 *  @}
 */