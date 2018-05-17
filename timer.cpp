#include <DueTimer.h>

#include "common.h"
#include "timer.h"
#include "pdo.h"
#include "CO_can.h"

#define SCU_FRONT_FIRST_SLOT 			0
#define SCU_FRONT_SECOND_SLOT 			1
#define SCU_REAR_SLOT 					2
#define TCS_SLOT 						3

#define TIME_SLOT_MASK                  3

volatile uint8_t		t_slot = SCU_FRONT_FIRST_SLOT;

DueTimer* co_timer;

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
    }
#elif defined(_RETRO_)
    switch (t_slot) {
		case SCU_REAR_SLOT: {
            Message m = Message_Initializer;
            buildPDO(PDO1tx, &m);
            canSend(&m);
			break;
        }
    }
#endif

    t_slot = (t_slot + 1) & TIME_SLOT_MASK;
}

__attribute__((__inline__)) void timerInit() {
	co_timer = &(DueTimer::getAvailable().attachInterrupt(TimeDispatch));
}

__attribute__((__inline__)) void timerStart() {
	co_timer -> start(TIME_SLOT_PERIOD);
}

__attribute__((__inline__)) void timerStop() {
	co_timer -> stop();
}
