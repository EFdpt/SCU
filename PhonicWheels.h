/** 
 *  @file    PhonicWheels.h
 *  @author  Arella Matteo
 *  @date    02/03/2018
 *  @version 1.0 
 *  
 *  @brief 
 *
 *  @section DESCRIPTION
 *  
 *	a
 *
 */

#ifndef _PHONIC_WHEELS_
#define _PHONIC_WHEELS_

#if !_USE_TIMING_REFERENCES_
#include <DueTimer.h>
#endif

#include "Encoder.h"

typedef enum _enPos {
	FR_SX,
	FR_DX,
	RT_SX,
	RT_DX
} enPos;

class PhonicWheels {
	private:
		Encoder       encoders[4];
		volatile bool start;
		unsigned long period;

  #if !_USE_TIMING_REFERENCES_
		DueTimer* 	  timer;
	#endif
  
		PhonicWheels();
		
	public:
		Encoder getEncoder(enPos encoder_position);
		bool begin();
		bool end();
		static PhonicWheels Instance();
  #if !_USE_TIMING_REFERENCES_
    void setFlushPeriod(unsigned long);
  #endif
};

#endif /* _PHONIC_WHEELS_ */
