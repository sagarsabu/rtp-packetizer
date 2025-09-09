#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <span>
#include <utility>
#include <vector>
#include "Rtcp/RtcpParser.hpp"
#include "Rtcp/RtcpApp.hpp"
#include "Rtcp/RtcpBye.hpp"
#include "Rtcp/RtcpHeader.hpp"
#include "Rtcp/RtcpPackets.hpp"
#include "Rtcp/RtcpSdes.hpp"
#include "Rtcp/RtcpSenderRr.hpp"

namespace rtp
{

using PktSpan = decltype(std::span{ std::vector<uint8_t>().cbegin(), std::vector<uint8_t>().cend() });
using DiffSize = std::vector<uint8_t>::difference_type;

std::optional<RtcpSenderReportPkt> ParseSenderReportPkt(PktSpan rawPkt)
{
    if (rawPkt.size() < sizeof(RtcpSenderReportHeader))
    {
        return std::nullopt;
    }

    RtcpSenderReportPkt pkt{};
    std::memcpy(&pkt.header, rawPkt.data(), sizeof(RtcpSenderReportHeader));

    // max 32 report blocks
    size_t nRRBlocks{ std::min<size_t>(pkt.header.cmnHdr.receptionCount, 31) };
    size_t expectedSizeWithRRBlocks{ (nRRBlocks * sizeof(RtcpReportBlock)) + sizeof(RtcpSenderReportHeader) };
    if (rawPkt.size() < expectedSizeWithRRBlocks)
    {
        return std::nullopt;
    }

    pkt.rrBlocks.reserve(nRRBlocks);

    for (auto itr{ rawPkt.begin() + sizeof(RtcpSenderReportHeader) };
         itr < rawPkt.begin() + static_cast<DiffSize>(expectedSizeWithRRBlocks);
         itr += sizeof(RtcpReportBlock))
    {
        std::span rawBlock{ itr, itr + sizeof(RtcpReportBlock) };
        RtcpReportBlock block{};
        std::memcpy(&block, rawBlock.data(), rawBlock.size());
        pkt.rrBlocks.emplace_back(block);
    }

    return std::make_optional(std::move(pkt));
}

std::optional<RtcpReceiverReportPkt> ParseReceiverReportPkt(PktSpan rawPkt)
{
    if (rawPkt.size() < sizeof(RtcpReceiverReportHeader))
    {
        return std::nullopt;
    }

    RtcpReceiverReportPkt pkt{};
    std::memcpy(&pkt.header, rawPkt.data(), sizeof(RtcpReceiverReportHeader));

    // max 32 report blocks
    size_t nRRBlocks{ std::min<size_t>(pkt.header.cmnHdr.receptionCount, 31) };
    size_t expectedSizeWithRRBlocks{ (nRRBlocks * sizeof(RtcpReportBlock)) + sizeof(RtcpReceiverReportHeader) };
    if (rawPkt.size() < expectedSizeWithRRBlocks)
    {
        return std::nullopt;
    }

    pkt.rrBlocks.reserve(nRRBlocks);

    for (auto itr{ rawPkt.begin() + sizeof(RtcpReceiverReportHeader) };
         itr < rawPkt.begin() + static_cast<DiffSize>(expectedSizeWithRRBlocks);
         itr += sizeof(RtcpReportBlock))
    {
        std::span rawBlock{ itr, itr + sizeof(RtcpReportBlock) };
        RtcpReportBlock block{};
        std::memcpy(&block, rawBlock.data(), rawBlock.size());
        pkt.rrBlocks.emplace_back(block);
    }

    return std::make_optional(std::move(pkt));
}

std::optional<RtcpSdesPkt> ParseSdesPkt(PktSpan rawPkt)
{
    if (rawPkt.size() < sizeof(RtcpSdesHeader))
    {
        return std::nullopt;
    }

    // TODO
    return std::nullopt;
}

std::optional<RtcpByePkt> ParseByePkt(PktSpan rawPkt)
{
    if (rawPkt.size() < sizeof(RtcpByeHeader))
    {
        return std::nullopt;
    }

    RtcpByePkt pkt{};
    std::memcpy(&pkt.header, rawPkt.data(), sizeof(RtcpByeHeader));
    return std::make_optional(pkt);
}

std::optional<RtcpAppPkt> ParseAppPkt(PktSpan rawPkt)
{
    if (rawPkt.size() < sizeof(RtcpAppHeader))
    {
        return std::nullopt;
    }

    RtcpAppPkt pkt{};
    std::memcpy(&pkt.header, rawPkt.data(), sizeof(RtcpAppHeader));
    pkt.data = { rawPkt.begin() + sizeof(RtcpAppHeader), rawPkt.end() };

    return std::make_optional(std::move(pkt));
}

std::vector<RtcpPktVariant> ParseRtcp(const std::vector<uint8_t>& fullPacket)
{
    std::vector<RtcpPktVariant> res{};

    auto pktItr{ fullPacket.begin() };
    while (pktItr < fullPacket.end())
    {
        PktSpan compoundPacket{ pktItr, fullPacket.end() };
        if (compoundPacket.size() < sizeof(RtcpHeader))
        {
            return {};
        }

        const auto* const cmnHeader{ reinterpret_cast<const RtcpHeader*>(compoundPacket.data()) };
        if (cmnHeader->version != 2)
        {
            return {};
        }

        // rfc3550#section-6.4.1
        auto pktSize{ static_cast<DiffSize>((cmnHeader->length + 1) * 4) };
        if ((pktItr + pktSize) > fullPacket.end())
        {
            return {};
        }

        // advance
        pktItr += pktSize;

        switch (cmnHeader->pktType)
        {
            case RtcpType::SenderRR:
            {
                if (auto pkt{ ParseSenderReportPkt(compoundPacket) })
                {
                    res.emplace_back(std::move(*pkt));
                }
                break;
            }
            case RtcpType::ReceiverRR:
            {
                if (auto pkt{ ParseReceiverReportPkt(compoundPacket) })
                {
                    res.emplace_back(std::move(*pkt));
                }
                break;
            }
            case RtcpType::Sdes:
            {
                if (auto pkt{ ParseSdesPkt(compoundPacket) })
                {
                    res.emplace_back(std::move(*pkt));
                }
                break;
            }
            case RtcpType::Bye:
            {
                if (auto pkt{ ParseByePkt(compoundPacket) })
                {
                    res.emplace_back(*pkt);
                }
                break;
            }
            case RtcpType::App:
            {
                if (auto pkt{ ParseAppPkt(compoundPacket) })
                {
                    res.emplace_back(std::move(*pkt));
                }
                break;
            }
            default:
            {
                // skip unknown
                break;
            }
        }
    }

    return res;
}

} // namespace rtp
