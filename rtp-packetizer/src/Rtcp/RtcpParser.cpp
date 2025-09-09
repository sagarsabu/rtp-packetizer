#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <utility>
#include <vector>
#include "Rtcp/RtcpParser.hpp"
#include "Rtcp/RtcpHeader.hpp"
#include "Rtcp/RtcpPackets.hpp"
#include "Rtcp/RtcpSenderRr.hpp"

namespace rtp
{

std::vector<RtcpPktVariant> ParseRtcp(const std::vector<uint8_t>& fullPacket)
{
    using diffSize_t = std::vector<uint8_t>::difference_type;

    std::vector<RtcpPktVariant> res{};

    auto pktItr{ fullPacket.begin() };
    while (pktItr < fullPacket.end())
    {
        std::span compoundPacket{ pktItr, fullPacket.end() };
        assert(compoundPacket.size() >= sizeof(RtcpHeader));

        const auto* const cmnHeader{ reinterpret_cast<const RtcpHeader*>(compoundPacket.data()) };
        assert(cmnHeader->version == 2);

        // rfc3550#section-6.4.1
        size_t pktSize{ static_cast<size_t>((cmnHeader->length + 1) * 4) };
        assert((pktItr + pktSize) < fullPacket.end());

        // advance
        pktItr += static_cast<diffSize_t>(pktSize);

        switch (cmnHeader->pktType)
        {
            case RtcpType::SenderRR:
            {
                assert(compoundPacket.size() >= sizeof(RtcpSenderReportHeader));

                RtcpSenderReportHeader header{};
                std::memcpy(&header, compoundPacket.data(), sizeof(RtcpSenderReportHeader));

                // max 32 report blocks
                size_t nRBlocks{ std::min<size_t>(header.cmnHdr.receptionCount, 31) };
                size_t expectedSizeWithRBlocks{ (nRBlocks * sizeof(RtcpReportBlock)) + sizeof(RtcpSenderReportHeader) };
                assert(compoundPacket.size() >= expectedSizeWithRBlocks);

                std::vector<RtcpReportBlock> blocks{};
                blocks.reserve(nRBlocks);

                for (auto itr{ compoundPacket.begin() + sizeof(RtcpSenderReportHeader) };
                     itr < compoundPacket.begin() + static_cast<diffSize_t>(expectedSizeWithRBlocks);
                     itr += sizeof(RtcpReportBlock))
                {
                    std::span rawBlock{ itr, itr + sizeof(RtcpSenderReportHeader) };
                    RtcpReportBlock block{};
                    std::memcpy(&block, rawBlock.data(), rawBlock.size());
                    blocks.emplace_back(block);
                }

                //  optional profile extention
                uint8_t optProfEtx{ 0 };
                if (compoundPacket.size() > expectedSizeWithRBlocks)
                {
                    optProfEtx = compoundPacket[expectedSizeWithRBlocks + 1];
                }

                res.emplace_back(RtcpSenderReportPkt{
                    .header = header,
                    .rrBlocks = std::move(blocks),
                    .optProfExt = optProfEtx,
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
