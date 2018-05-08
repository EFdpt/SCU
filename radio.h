#ifndef _RADIO_H_
#define _RADIO_H_

#define RADIO_KEY_BITS	192 // choose 128, 192 or 256
#define RADIO_IV		36753562

byte RADIO_KEY[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

#define GET_CIPHER_LENGTH(str_length) 			\
		(str_length + (N_BLOCK - ((str_length - 1) % 16)))

int encrypt_model(byte* model, size_t model_length, byte* cipher, size_t cipher_length);

#endif /* _RADIO_H_ */