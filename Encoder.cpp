#include "Encoder.h"

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

void Encoder::setHolesNumber(uint8_t num) {
	if (!this -> started)
		this -> holes_number = num;
}

uint8_t Encoder::getHolesNumber() {
	return this -> holes_number;
}

bool Encoder::start() {
  if (this -> pin < 0 || !this -> holes_number || this -> started)
    return false;
  return (this -> started = true);
}

volatile bool Encoder::isStarted() {
	return this -> started;
}

void Encoder::addPulse() {
	if (!this -> started)
		return;
	this -> pulses++;
}
		
void Encoder::resetPulses() {
	if (!this -> started)
		return;
	this -> pulses = 0;
}

uint16_t Encoder::getFrequency() {
	return 0;
}

uint16_t Encoder::getRPM() {
	return 0;
}

