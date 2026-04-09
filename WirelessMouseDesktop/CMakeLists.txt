cmake_minimum_required(VERSION 4.3)
project("WirelessMouseDesktop")

set(CMAKE_EXPORT_COMPILE_COMMANDS "yes")
add_compile_options(
    -Wall
    -Werror
    -Wextra
    -Wpedantic
    -O2
)
add_executable(WirelessMouseDesktop)
add_subdirectory(src)

install(TARGETS WirelessMouseDesktop DESTINATION /usr/bin/)
install(FILES WirelessMouseDesktop.service DESTINATION /etc/systemd/system/)
