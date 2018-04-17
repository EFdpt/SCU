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
  Serial.print(" (");
  Serial.print(map(tps1_value, tps1_low, tps1_max, 0, 100));
  Serial.print("%) [");
  Serial.print(tps1_low);
  Serial.print(", ");
  Serial.print(tps1_max);
  Serial.print("] ||| tps2 = ");
  Serial.print(tps2_value);
  Serial.print(" (");
  Serial.print(map(tps2_value, tps2_low, tps2_max, 0, 100));
  Serial.print("%) [");
  Serial.print(tps2_low);
  Serial.print(", ");
  Serial.print(tps2_max);
  Serial.print("] ||| brake = ");
  Serial.print(brake_value);
  Serial.print(" (");
  Serial.print(map(brake_value, brake_low, brake_max, 0, 100));
  Serial.print("%) [");
  Serial.print(brake_low);
  Serial.print(", ");
  Serial.print(brake_max);
  Serial.print("] ||| fr_sx_susp = ");
  Serial.print(fr_sx_susp);
  Serial.print(" ||| fr_dx_susp = ");
  Serial.print(fr_dx_susp);
  if (!plaus)
    Serial.println(" ||| plaus error");
  else Serial.println(" ||| plaus OK");
}
