#include "PhonicWheels.h"

//PhonicWheels instance;

PhonicWheels PhonicWheels::Instance() {
  static PhonicWheels instance;
  return instance;
}

#if !_USE_TIMING_REFERENCES_
void tim_handler() {
	PhonicWheels::Instance().getEncoder(FR_SX).resetPulses();
	PhonicWheels::Instance().getEncoder(FR_DX).resetPulses();
	PhonicWheels::Instance().getEncoder(RT_SX).resetPulses();
	PhonicWheels::Instance().getEncoder(RT_DX).resetPulses();
}
#endif

void exti_handler0() {
	PhonicWheels::Instance().getEncoder(FR_SX).pulse();
}

void exti_handler1() {
	PhonicWheels::Instance().getEncoder(FR_DX).pulse();
}

void exti_handler2() {
	PhonicWheels::Instance().getEncoder(RT_SX).pulse();
}

void exti_handler3() {
	PhonicWheels::Instance().getEncoder(RT_DX).pulse();
}

PhonicWheels::PhonicWheels() {
	this -> start = false;
 #if !_USE_TIMING_REFERENCES_
	this -> timer = &(DueTimer::getAvailable().attachInterrupt(tim_handler));
 #endif
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
 #if !_USE_TIMING_REFERENCES_
	this -> timer -> start(this -> period);
 #endif
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

 #if !_USE_TIMING_REFERENCES_
	this -> timer -> stop();
 #endif
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

#if !_USE_TIMING_REFERENCES_
void PhonicWheels::setFlushPeriod(unsigned long period) {
  if (this -> start || !period)
    return;

  this -> period = period;
    
  this -> encoders[0].setFlushPeriod(pediod);
  this -> encoders[1].setFlushPeriod(pediod);
  this -> encoders[2].setFlushPeriod(pediod);
  this -> encoders[3].setFlushPeriod(pediod);
}
#endif
