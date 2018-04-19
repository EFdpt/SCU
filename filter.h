#ifndef FILTER_H_
#define FILTER_H_

#include <stdint.h>
#include "model.h"

uint16_t filter_buffer(volatile uint16_t* buffer, int size, unsigned offset);

#endif /* FILTER_H_ */
