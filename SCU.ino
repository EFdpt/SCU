#include <due_can.h>
#include <DueTimer.h>
#include "model.h"

void setup() {
  model_init();
  model_enable_calibrations();
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);
}

void loop() {
  RADIO_send_model();
}
