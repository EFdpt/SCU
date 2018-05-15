#include "common.h"
#include "CAN_ID.h"
#include "model.h"

#include "CANOpen/states.h"
#include "CANOpen/nmt.h"

void setup() {
    // Serial.begin(SERIAL_BAUDRATE);
    // while(!Serial);

    setNodeId(NODE_ID);
    initialisation();
    slaveSendBootUp();  
}

void loop() {
}
