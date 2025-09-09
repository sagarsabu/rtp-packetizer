cmake_minimum_required(VERSION 3.10.0)
project(
  rtp-packetizer
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)


add_subdirectory(rtp-packetizer)
add_subdirectory(app)
