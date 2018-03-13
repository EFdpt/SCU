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

#include <DueTimer.h>
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
		DueTimer* 	  timer;
		
		PhonicWheels();
		
	public:
		void setFlushPeriod(unsigned long);
		Encoder getEncoder(enPos encoder_position);
		
		bool begin();
		bool end();
   
		static PhonicWheels Instance();
};

#endif /* _PHONIC_WHEELS_ */
