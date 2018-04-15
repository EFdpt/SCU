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
  Serial.print("tps1 = ");
  Serial.print(tps1_value);
  Serial.print(", tps1_low = ");
  Serial.print(tps1_low);
  Serial.print(", tps1_max = ");
  Serial.print(tps1_max);
  Serial.print(" |||| tps2 = ");
  Serial.print(tps2_value);
  Serial.print(", tps2_low = ");
  Serial.print(tps2_low);
  Serial.print(", tps2_max = ");
  Serial.println(tps2_max);
}
