#ifndef _RADIO_H_
#define _RADIO_H_

#define RADIO_KEY		"70dc0a27994bdf55b936ac459628783f0d49423ee1356f58f9700055081ae61c"
#define RADIO_KEY_BITS	192 // choose 128, 192 or 256
#define RADIO_IV		36753562

#define GET_CIPHER_LENGTH(str_length) 			\
		(str_length + (N_BLOCK - ((str_length - 1) % 16)))

void encrypt_model(String, byte* cipher, size_t cipher_length);

#endif /* _RADIO_H_ */