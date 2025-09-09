cmake_minimum_required(VERSION 3.10.0)
project(
  rtp-sandbox
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.15.3)
set(SPDLOG_USE_STD_FORMAT OFF)
set(SPDLOG_BUILD_PIC ON)
set(SPDLOG_BUILD_EXAMPLE OFF)
FetchContent_MakeAvailable(spdlog)

file(GLOB SRCS src/*.cpp)
add_executable(rtp-sandbox ${SRCS})

target_compile_options(rtp-sandbox PRIVATE -Wall -Wextra -Werror -Wpedantic)
target_include_directories(rtp-sandbox PRIVATE src/)
target_link_libraries(rtp-sandbox PRIVATE spdlog::spdlog)
