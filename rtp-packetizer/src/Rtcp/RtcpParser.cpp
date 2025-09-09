#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>
#include "Rtcp/RtcpParser.hpp"
#include "Rtcp/RtcpHeader.hpp"
#include "Rtcp/RtcpPackets.hpp"
#include "Rtcp/RtcpSenderRr.hpp"

namespace rtp
{

std::vector<RtcpPktVariant> ParseRtcp(const std::vector<uint8_t>& fullPacket)
{
    std::vector<RtcpPktVariant> res{};

    auto pktItr{ fullPacket.begin() };
    while (pktItr < fullPacket.end())
    {
        std::span compoundPacket{ pktItr, fullPacket.end() };
        assert(compoundPacket.size() >= sizeof(RtcpHeader));

        const auto cmnHeader{ reinterpret_cast<const RtcpHeader*>(compoundPacket.data()) };
        assert(cmnHeader->version == 2);

        // rfc3550#section-6.4.1
        size_t pktSize{ static_cast<size_t>((cmnHeader->length + 1) * 4) };
        assert((pktItr + pktSize) < fullPacket.end());

        // advance
        pktItr += static_cast<ssize_t>(pktSize);

        switch (cmnHeader->pktType)
        {
            case RtcpType::SenderRR:
            {
                assert(compoundPacket.size() >= sizeof(RtcpSenderReportHeader));

                const auto header{ reinterpret_cast<const RtcpSenderReportHeader*>(compoundPacket.data()) };
                // max 32 report blocks
                size_t nReportBlocks{ std::min<size_t>(header->cmnHdr.receptionCount, 31) };
                size_t expectedSize{ (nReportBlocks * sizeof(RtcpReportBlock)) + sizeof(RtcpSenderReportHeader) };
                assert(expectedSize >= compoundPacket.size());

                std::vector<RtcpReportBlock> blocks{};
                blocks.reserve(nReportBlocks);

                for (auto itr{ compoundPacket.begin() + sizeof(RtcpSenderReportHeader) };
                     itr < compoundPacket.begin() + expectedSize;
                     itr += sizeof(RtcpReportBlock))
                {
                    const auto block{ reinterpret_cast<const RtcpReportBlock*>(*itr) };
                    blocks.emplace_back(*block);
                }

                res.emplace_back(RtcpSenderReportPkt{
                    .header = *header,
                    .rrBlocks = std::move(blocks),
                    .optProfExt = 0,
                });
                break;
            }
            case RtcpType::ReceiverRR:
            {
                break;
            }
            case RtcpType::Sdes:
            {
                break;
            }
            case RtcpType::Bye:
            {
                break;
            }
            case RtcpType::App:
            {
                break;
            }
            default:
            {
                assert(false);
                break;
            }
        }
    }

    return res;
}

} // namespace rtp
