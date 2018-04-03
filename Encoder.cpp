#include <Arduino.h>
#include "Encoder.h"

#if _USE_TIMING_REFERENCES_
  #define __update_sample_interval__      \
    this -> sample_interval = (60*1000/num);
#else
  #define __update_sample_interval__      \
    this -> sample_interval = ((60*1000)/(num * this -> flush_period));
#endif

Encoder::Encoder() {
	this -> pin = -1;
}

void Encoder::attachPin(int pin) {
  if (!this -> started && pin >= 0)
    this -> pin = pin;
}

int Encoder::getPin() {
	return this -> pin;
}

void Encoder::setCogsNumber(uint8_t num) {
	if (this -> started || !num)
    return;
	this -> cogs_number = num;
  
  __update_sample_interval__
}

uint8_t Encoder::getCogsNumber() {
	return this -> cogs_number;
}

bool Encoder::start() {
 #if !_USE_TIMING_REFERENCES_
  if (this -> started || this -> pin < 0 || !this -> cogs_number || !this -> flush_period)
 #else
  if (this -> started || this -> pin < 0 || !this -> cogs_number)
 #endif
    return false;
  return (this -> started = true);
}

bool Encoder::stop() {
  return !(this -> started = false);
}

volatile bool Encoder::isStarted() {
	return this -> started;
}

void Encoder::pulse() {
	if (!this -> started)
		return;
 #if _USE_TIMING_REFERENCES_
  this -> prev_time = this -> curr_time;
  this -> curr_time = micros();
 #else
	this -> pulses++;
 #endif
}

#if !_USE_TIMING_REFERENCES_
volatile uint16_t Encoder::getPulses() {
  if (!this -> started)
    return 0;
  return this -> pulses;
}

void Encoder::resetPulses() {
  if (!this -> started)
    return;
  this -> pulses = 0;
}

void Encoder::setFlushPeriod(unsigned long period) {
  if (this -> started || !period)
    return;

  this -> flush_period = period;
  
  __update_sample_interval__
}

uint16_t Encoder::getFrequency() {
  return 0;
}
#endif

uint16_t Encoder::getRPM() {
	if (!this -> started)
    return 0;
  #if _USE_TIMING_REFERENCES_
    return this -> sample_interval / (this -> curr_time - this -> prev_time);
  #else
    return this -> sample_interval * this -> pulses;
  #endif
}

