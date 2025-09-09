#pragma once

#include "Rtcp/RtcpPackets.hpp"

namespace rtp
{

std::vector<RtcpPktVariant> ParseRtcp(const std::vector<uint8_t>& fullPacket);

}
