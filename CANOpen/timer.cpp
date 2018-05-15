#include <DueTimer.h>

#include "../common.h"
#include "timer.h"
#include "pdo.h"
#include "CO_can.h"

#define SCU_FRONT_FIRST_SLOT 			0
#define SCU_FRONT_SECOND_SLOT 			1
#define SCU_REAR_SLOT 					2
#define TCS_SLOT 						3

#define TIME_SLOT_MASK                  3

volatile uint8_t		t_slot = SCU_FRONT_FIRST_SLOT;

DueTimer* timer;

void TimeDispatch() { // send PDOs periodically
	switch (t_slot) {

#if defined(_FRONTAL_)
		case SCU_FRONT_FIRST_SLOT:
			Message m = Message_Initializer;
            buildPDO(PDO1tx, &m);
            canSend(&m);
			break;
		case SCU_FRONT_SECOND_SLOT:
            Message m = Message_Initializer;
            buildPDO(PDO2tx, &m);
            canSend(&m);
			break;

#elif defined(_RETRO_)
		case SCU_REAR_SLOT:
            Message m = Message_Initializer;
            buildPDO(PDO1tx, &m);
            canSend(&m);
			break;
#endif
		default: t_slot = (t_slot + 1) & TIME_SLOT_MASK;
	}
}

inline void timerInit() {
	*timer = DueTimer::getAvailable().attachInterrupt(TimeDispatch).setPeriod(TIME_SLOT_PERIOD);
}

inline void timerStart() {
	timer -> start();
}

inline void timerStop() {
	timer -> stop();
}
