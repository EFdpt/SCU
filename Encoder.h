#ifndef _ENCODER_
#define _ENCODER_

#include <stdint.h>

#define _USE_TIMING_REFERENCES_   1

class Encoder {
	private:
		int 				      pin;
		uint8_t				    cogs_number;
    uint8_t           sample_interval;
		volatile bool		  started;
  #if _USE_TIMING_REFERENCES_
    volatile unsigned long prev_time;
    volatile unsigned long curr_time;
  #else
		volatile uint16_t pulses;
    unsigned long flush_period;
  #endif
  
  public:
		Encoder();
		
		void attachPin(int);
		int getPin();
		void setCogsNumber(uint8_t);
		uint8_t getCogsNumber();
		bool start();
		bool stop();
		volatile bool isStarted();
		
		void pulse();

 #if !_USE_TIMING_REFERENCES_
    volatile uint16_t getPulses();
    void resetPulses();
    void setFlushPeriod(unsigned long);
    uint16_t getFrequency();
 #endif
		
		uint16_t getRPM();
    uint16_t getAngularVelocity();
};

#endif /* _ENCODER_ */
