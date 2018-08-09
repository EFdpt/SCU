/** 
 *  @file           radio_packet.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          Radio packet header file
 */

#ifndef _RADIO_PACKET_H_
#define _RADIO_PACKET_H_

#include <stdint.h>

struct radio_header {
    uint16_t id;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t frag_offset:7,
            mf:1;
#else
    uint8_t mf:1,
            frag_offset:7;
#endif
    uint8_t data_length;
};

#define RADIO_DF 0            /* dont fragment flag */
#define RADIO_MF 1            /* more fragments flag */

#define MTU                 255 // maximum trasmission unit (bytes)
#define RADIO_HEADER_SIZE   4 // bytes

struct radio_pkt {
    struct radio_header h;
    uint8_t data[MTU - RADIO_HEADER_SIZE];
};

#endif /* _RADIO_PACKET_H_ */