#pragma once

#include "rtcp_packets.hpp"

std::vector<RtcpPktVariant> Parse(const std::vector<uint8_t>& fullPacket);
