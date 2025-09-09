#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include "Rtcp/RtcpApp.hpp"
#include "Rtcp/RtcpBye.hpp"
#include "Rtcp/RtcpHeader.hpp"
#include "Rtcp/RtcpReceiverRr.hpp"
#include "Rtcp/RtcpSdes.hpp"
#include "Rtcp/RtcpSenderRr.hpp"

namespace rtp
{

struct RtcpSenderReportPkt
{
    RtcpSenderReportHeader header;
    std::vector<RtcpReportBlock> rrBlocks;
    uint8_t optProfExt;
};

struct RtcpReceiverReportPkt
{
    RtcpReceiverReportHeader header;
    std::vector<RtcpReportBlock> blocks;
    uint32_t optProfExt;
};

struct RtcpSdesVariantCname
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Cname };

    std::string cname;
};

struct RtcpSdesVariantUsername
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Username };

    std::string username;
};

struct RtcpSdesVariantEmail
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Email };

    std::string email;
};

struct RtcpSdesVariantPhone
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Phone };

    std::string phone;
};

struct RtcpSdesVariantLoc
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Loc };

    std::string loc;
};

struct RtcpSdesVariantTool
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Tool };

    std::string tool;
};

struct RtcpSdesVariantNote
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Note };

    std::string note;
};

struct RtcpSdesVariantPriv
{
    static constexpr RtcpSdesType s_type{ RtcpSdesType::Priv };

    std::string prefixStr;
    std::string valStr;
};

using RtcpSdesVariant = std::variant<
    RtcpSdesVariantCname, RtcpSdesVariantUsername, RtcpSdesVariantEmail, RtcpSdesVariantPhone, RtcpSdesVariantLoc,
    RtcpSdesVariantNote, RtcpSdesVariantPriv>;

struct RtcpSdesPkt
{
    RtcpSdesHeader header;
    std::vector<RtcpSdesVariant> sdeItems;
};

struct RtcpByePkt
{
    RtcpBye header;
    std::string optLeaveReason;
};

struct RtcpAppPkt
{
    RtcpApp header;
    std::vector<uint8_t> data;
};

using RtcpPktVariant = std::variant<RtcpSenderReportPkt, RtcpReceiverReportPkt, RtcpSdesPkt, RtcpByePkt, RtcpAppPkt>;

} // namespace rtp
