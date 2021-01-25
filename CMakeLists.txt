cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE_FILE mav_sdk.cpp mav_sdk.h)

project(Access_Sensor)

if(NOT MSVC)
    add_definitions("-Wall -Wextra")
else()
    add_definitions("-WX -W2")
endif()

find_package(MAVSDK REQUIRED)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Access_Sensor
    Access_Sensor.cpp
    mav_sdk.cpp
)

target_link_libraries(Access_Sensor
    ${CURSES_LIBRARIES}
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk_calibration
    MAVSDK::mavsdk
    
)
