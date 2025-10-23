cmake_minimum_required(VERSION 3.20)
project(SmartTV-Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Try to find asio first (system install)
find_package(asio QUIET)

if(NOT asio_FOUND)
    message(STATUS "asio not found, fetching with FetchContent...")

    FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-36-0
    )
    FetchContent_MakeAvailable(asio)
endif()

# spdlog
find_package(spdlog QUIET)

if (NOT spdlog_FOUND)
    message(STATUS "spdlog not found, fetching with FetchContent")

    FetchContent_Declare(
            spdlog
            GIT_REPOSITORY https://github.com/gabime/spdlog.git
            GIT_TAG v1.16.0
    )
    FetchContent_MakeAvailable(spdlog)
endif()

add_executable(tv-server
        src/ClientManager.cpp
        src/Log.cpp
        src/main.cpp
        src/Session.cpp
        src/SmartTV.cpp

        include/SmartTV/ClientManager.h
        include/SmartTV/Log.h
        include/SmartTV/Session.h
        include/SmartTV/SmartTV.h
)

# Asio is header-only, so we don't need to link a library, just include the headers
target_include_directories(tv-server PUBLIC include ${asio_SOURCE_DIR}/asio/include)

target_link_libraries(tv-server PUBLIC spdlog::spdlog)

if(UNIX)
    target_link_libraries(tv-server PRIVATE pthread)
endif()
