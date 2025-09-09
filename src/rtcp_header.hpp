#pragma once

#include <cstdint>
#include <endian.h>

enum RtcpType : uint8_t
{
    SenderRR = 200,
    ReceiverRR = 201,
    Sdes = 202,
    Bye = 203,
    App = 204,
};

/**
Common RTCP Header

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|    RC   |   PT          |             length            |
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*/

struct [[gnu::packed]] RtcpHeader
{
#if __BYTE_ORDER == __BIG_ENDIAN
    uint8_t version        : 2;
    uint8_t padding        : 1;
    uint8_t receptionCount : 5;
#else
    uint8_t receptionCount : 5;
    uint8_t padding        : 1;
    uint8_t version        : 2;
#endif
    uint8_t pktType;
    uint16_t length;
};

/**
RTCP Report Block for Sender Report & Receiver Report

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                 SSRC_1 (SSRC of first source)                 |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| fraction lost |       cumulative number of packets lost       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           extended highest sequence number received           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                      interarrival jitter                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         last SR (LSR)                         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                   delay since last SR (DLSR)                  |
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*/

struct [[gnu::packed]] RtcpReportBlock
{
    uint32_t ssrc;
    uint8_t fractionLost;
    uint32_t cumNumPktsLost : 24;
    uint32_t extHighestSeqNumRx;
    uint32_t intervalJitter;
    uint32_t lastSr;
    uint32_t delayLastSr;
};

#if __BYTE_ORDER == __BIG_ENDIAN

#else

#endif
