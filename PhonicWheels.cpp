#include "PhonicWheels.h"

PhonicWheels PhonicWheels::Instance() {
  static PhonicWheels instance;
  return instance;
}

void tim_handler() {
	PhonicWheels::Instance().getEncoder(FR_SX).resetPulses();
	PhonicWheels::Instance().getEncoder(FR_DX).resetPulses();
	PhonicWheels::Instance().getEncoder(RT_SX).resetPulses();
	PhonicWheels::Instance().getEncoder(RT_DX).resetPulses();
}

void exti_handler0() {
	PhonicWheels::Instance().getEncoder(FR_SX).addPulse();
}

void exti_handler1() {
	PhonicWheels::Instance().getEncoder(FR_DX).addPulse();
}

void exti_handler2() {
	PhonicWheels::Instance().getEncoder(RT_SX).addPulse();
}

void exti_handler3() {
	PhonicWheels::Instance().getEncoder(RT_DX).addPulse();
}

PhonicWheels::PhonicWheels() {
	this -> start = false;
	this -> timer = &(DueTimer::getAvailable().attachInterrupt(tim_handler));
}

bool PhonicWheels::begin() {
	if (this -> start || !this -> period)
		return false;
	
	Encoder e;
	bool started = false;
	
	if ((e = this -> encoders[0]).start()) {
		attachInterrupt(digitalPinToInterrupt(e.getPin()), exti_handler0, FALLING);
		started = true;
	}
	if ((e = this -> encoders[1]).start()) {
		attachInterrupt(digitalPinToInterrupt(e.getPin()), exti_handler1, FALLING);
		started = true;
	}
	if ((e = this -> encoders[2]).start()) {
		attachInterrupt(digitalPinToInterrupt(e.getPin()), exti_handler2, FALLING);
		started = true;
	}
	if ((e = this -> encoders[3]).start()) {
		attachInterrupt(digitalPinToInterrupt(e.getPin()), exti_handler3, FALLING);
		started = true;
	}
	
	if (!started)
		return false;
		
	this -> start = true;
	this -> timer -> start(this -> period);
}

Encoder PhonicWheels::getEncoder(enPos pos) {
	if (pos < 0 || pos > 3)
		return this -> encoders[0];
	return this -> encoders[pos];
}

bool PhonicWheels::end() {
	if (!this -> start)
		return false;
	
	Encoder e;
	
	this -> timer -> stop();
	
	if ((e = this -> encoders[0]).isStarted()) {
		e.stop();
		detachInterrupt(digitalPinToInterrupt(e.getPin()));
	}
	if ((e = this -> encoders[1]).isStarted()) {
		e.stop();
		detachInterrupt(digitalPinToInterrupt(e.getPin()));
	}
	if ((e = this -> encoders[2]).isStarted()) {
		e.stop();
		detachInterrupt(digitalPinToInterrupt(e.getPin()));
	}
	if ((e = this -> encoders[3]).isStarted()) {
		e.stop();
		detachInterrupt(digitalPinToInterrupt(e.getPin()));
	}
	
	return true;
}

void PhonicWheels::setFlushPeriod(unsigned long period) {
  if (this -> start || !period)
    return;

  this -> period = period;
}
