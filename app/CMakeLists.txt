include(${CMAKE_SOURCE_DIR}/cmake/third-party/spdlog.cmake)

file(GLOB SRCS src/*.cpp)
add_executable(rtp-sandbox ${SRCS})

target_compile_options(rtp-sandbox PRIVATE -Wall -Wextra -Werror -Wpedantic)
target_link_libraries(rtp-sandbox PRIVATE spdlog::spdlog rtp-packetizer)
