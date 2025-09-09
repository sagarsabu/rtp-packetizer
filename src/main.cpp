#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int, char**)
{
    spdlog::set_default_logger(spdlog::stdout_color_mt("def"));
    spdlog::info("starting");
}
