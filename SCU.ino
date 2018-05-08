#include <DueTimer.h>
#include "model.h"

void setup() {
  model_init();

  #if defined(_FRONTAL_)
  model_enable_calibrations();
  #endif

  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);
}

void loop() {
}
