#ifndef _RADIO_H_
#define _RADIO_H_

#include "common.h"

#if defined(_RETRO_)

    #define CTR 1

    #include "aes.h"

    #define RADIO_KEY_BITS	192 // choose 128, 192 or 256
    #define RADIO_IV		36753562

    unsigned char key[AES_KEYLEN] = {   0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 
                                        0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 
                                        0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b }; // 24 bytes

    unsigned char IV[16] = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };

    //#define GET_CIPHER_LENGTH(str_length) 			\
    		(str_length + (N_BLOCK - ((str_length - 1) % 16)))

    int encrypt_model(byte* buffer, size_t model_length, size_t cipher_length);

    void radio_init();

    void radio_send_model();

#endif

#endif /* _RADIO_H_ */