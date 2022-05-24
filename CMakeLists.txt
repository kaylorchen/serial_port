cmake_minimum_required(VERSION 3.16)
project(serial_port)

set(CMAKE_CXX_STANDARD 14)

add_executable(serial_port main.cpp time_stamp.cpp time_stamp.h serial_port_stream.cpp serial_port_stream.h serial_port.cpp serial_port.h ptz_control.cpp ptz_control.h)

target_link_libraries(serial_port pthread)
