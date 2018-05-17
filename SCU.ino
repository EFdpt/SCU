#include "common.h"
#include "states.h"
#include "nmt.h"

void setup() {
    // Serial.begin(SERIAL_BAUDRATE);
    // while(!Serial);

    setNodeId(NODE_ID);
    initialisation();
    slaveSendBootUp();  
}

void loop() {
}
