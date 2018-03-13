#ifndef _ENCODER_
#define _ENCODER_

#include <stdint.h>

class Encoder {
	private:
		int 				pin;
		uint8_t				holes_number;
		volatile bool		started;
		volatile uint16_t 	pulses;

  public:
		Encoder();
		
		void attachPin(int);
		int getPin();
		void setHolesNumber(uint8_t);
		uint8_t getHolesNumber();
		bool start();
		void stop();
		volatile bool isStarted();
		
		void addPulse();
		void resetPulses();
		void getPulses();
		
		uint16_t getFrequency();
		uint16_t getRPM();
};

#endif /* _ENCODER_ */
