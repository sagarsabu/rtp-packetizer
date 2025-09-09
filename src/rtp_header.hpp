#pragma once

#include <cstdint>
#include <endian.h>

/**

RTP Header

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X|  CC   |M|     PT      |       sequence number         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           timestamp                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           synchronization source (SSRC) identifier            |
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
|            contributing source (CSRC) identifiers             |
|                             ....                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/

struct [[gnu::packed]] RptHeader
{
#if __BYTE_ORDER == __BIG_ENDIAN
    uint8_t version : 2;
    uint8_t padding : 1;
    uint8_t ext     : 1;
    uint8_t cc      : 4;
#else
    uint8_t cc      : 4;
    uint8_t ext     : 1;
    uint8_t padding : 1;
    uint8_t version : 2;
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
    uint8_t marker  : 1;
    uint8_t pktType : 7;
#else
    uint8_t pktType : 7;
    uint8_t marker  : 1;
#endif

    uint16_t seq;
    uint32_t ts;
    uint32_t ssrc;
    uint32_t csrc;
};

#if __BYTE_ORDER == __BIG_ENDIAN

#else

#endif
