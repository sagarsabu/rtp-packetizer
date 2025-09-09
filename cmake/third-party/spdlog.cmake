FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.15.3)
set(SPDLOG_USE_STD_FORMAT OFF)
set(SPDLOG_BUILD_PIC ON)
set(SPDLOG_BUILD_EXAMPLE OFF)
FetchContent_MakeAvailable(spdlog)
