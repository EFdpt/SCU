#ifndef _SORT_H_
#define _SORT_H_

#include <stdint.h>

#define SORT_ORDER_INCREASING	(1)

/*
 * Sorting algorithm
 */
void sort(uint16_t* arr, unsigned n);

/*
 * Sorting algorithm with offset indexing.
 */
void sort_off(uint16_t* arr, unsigned n, unsigned offset);

#endif /* _SORT_H_ */
