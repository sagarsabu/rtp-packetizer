#include <Rtcp/RtcpParser.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_default_logger(spdlog::stdout_color_mt("def"));
    spdlog::info("starting");

    auto res{ Parse({ 0x80, 0x00, 0x00, 0x00 }) };
    spdlog::info("{}", res.at(0).index());
}
