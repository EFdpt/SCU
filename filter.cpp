#include "filter.h"
#include "sort.h"

#define FILTER_BOUND			(8)

#define USE_LOOP_UNROLLING		(1)

#define LOOP_UNROLLING_SIZE		(4)

#define pos(x, offset)      ((x) * offset)

/**
 *  @author		Arella Matteo
 *  @brief		This function filters the input buffer with an average filter.
 *  @param		buffer [in] The buffer to filter
 *  @param		size [in] The size of the buffer
 *  @retval		Filtered value
 */
uint16_t filter_buffer(uint16_t* buffer, int size, unsigned offset) {
#if 0
	int start = FILTER_BOUND;
	int end = size - (FILTER_BOUND << 2);
	int divisor = end - start;
	if (divisor <= 0)
		divisor = 1;

	volatile int index = start;

	sort_off((uint16_t*) buffer, size, offset);

#if USE_LOOP_UNROLLING
	volatile long long sum1 = 0LL;
	volatile long long sum2 = 0LL;
	volatile long long sum3 = 0LL;
	volatile long long sum4 = 0LL;

	for (; index + LOOP_UNROLLING_SIZE < end; index += LOOP_UNROLLING_SIZE) {
		sum1 += buffer[pos(index, offset)];
		sum2 += buffer[pos(index + 1, offset)];
		sum3 += buffer[pos(index + 2, offset)];
		sum4 += buffer[pos(index + 3, offset)];
	}

	for (; index < end; index++)
		sum1 += buffer[pos(index, offset)];

	return (sum1 + sum2 + sum3 + sum4) / divisor;
#else
	volatile long long sum = 0LL;

	for (; index < end; index++) {
		sum += buffer[pos(index, offset)];
	}
	return sum / divisor;
#endif
#else

  volatile int index = 0;
  volatile unsigned long long sum = 0ULL;
  
  for (; index < size; index++)
    sum += buffer[pos(index, offset)];

  return sum / size;
#endif
}
