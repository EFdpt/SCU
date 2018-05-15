#ifndef _RADIO_H_
#define _RADIO_H_

#include "tiny-AES/aes.h"

#define RADIO_KEY_BITS	192 // choose 128, 192 or 256
#define RADIO_IV		36753562

unsigned char key[AES_KEYLEN] = "statuttosuldrivexxxxxxxx"; // 24 bytes
unsigned char IV[AES_KEYLEN] = "vhdNaleuTHenaOlLvhdNaleu";	// 24 bytes

//#define GET_CIPHER_LENGTH(str_length) 			\
		(str_length + (N_BLOCK - ((str_length - 1) % 16)))

int encrypt_model(byte* model, size_t model_length, byte* cipher, size_t cipher_length);

void radio_send_model()

#endif /* _RADIO_H_ */