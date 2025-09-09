file(GLOB SRCS src/Rtcp/*.cpp src/Rtp/*.cpp)
add_library(rtp-packetizer ${SRCS})

target_compile_options(rtp-packetizer PRIVATE -Wall -Wextra -Werror -Wpedantic)
target_include_directories(rtp-packetizer PUBLIC src/)
