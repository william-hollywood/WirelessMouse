# CMakeList.txt : CMake project for WirelessMouseDesktopV2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (WirelessMouseDesktopV2 "WirelessMouseDesktopV2.c" "WirelessMouseDesktopV2.h" "Encoding.c" "Encoding.h" "MouseControl.c" "MouseControl.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WirelessMouseDesktopV2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
