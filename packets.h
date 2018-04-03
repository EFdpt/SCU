#ifndef _PACKETS_H_
#define _PACKETS_H_

typedef struct phonic_wheels_data_s {
	uint16_t FR_SX;
	uint16_t FR_DX;
	uint16_t RT_SX;
	uint16_t RT_DX;
} p_w_data_t;

typedef union phonic_wheels_pack_s {
	uint64_t value;
	p_w_data_t data;
} p_w_pack_t;



#endif /* _PACKETS_H_ */
