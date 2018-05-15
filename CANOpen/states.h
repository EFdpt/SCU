#ifndef _STATES_H_
#define _STATES_H_

enum enum_nodeState {
    Initialisation = 0x00,
    Disconnected = 0x01,
    Connecting = 0x02,
    Preparing = 0x02,
    Stopped = 0x04,
    Operational = 0x05,
    Pre_operational = 0x7F,
    Unknown_state = 0x0F
} __attribute__ ((packed));

typedef enum enum_nodeState e_nodeState;

/** 
 * @brief Function that user app can overload
 * @ingroup statemachine
 */
void initialisation();
void preOperational();
void operational();
void stopped();

/** 
 * @brief Called by driver/app when receiving messages
 * @param *d Pointer on a CAN object data structure
 * @param *m Pointer on a CAN message structure
 */
void canDispatch(Message *m);

e_nodeState getState();

void setState(e_nodeState newState);

uint8_t getNodeId();

#endif /* _STATES_H_ */