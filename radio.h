#ifndef _RADIO_H_
#define _RADIO_H_

#include "common.h"

#if defined(_RETRO_)

    #define CTR 1

    #include "aes.h"

    #define RADIO_KEY_BITS	192 // choose 128, 192 or 256

    extern uint8_t key[];

    extern volatile bool radio_transmit;

    void encrypt_model(char* buffer, uint16_t plain_len, uint16_t buffer_len);

    void radio_init();

    void radio_send_model();

#endif

#endif /* _RADIO_H_ */