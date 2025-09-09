#pragma once

#include "Rtcp/RtcpPackets.hpp"

std::vector<RtcpPktVariant> Parse(const std::vector<uint8_t>& fullPacket);
