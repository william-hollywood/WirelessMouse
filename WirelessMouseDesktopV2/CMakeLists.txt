cmake_minimum_required(VERSION 3.28)
project("WirelessMouseDesktop")

enable_language("RC")
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

file(GLOB ICONS "${CMAKE_CURRENT_LIST_DIR}/icons/*")
add_custom_command(
    OUTPUT
        ${CMAKE_BINARY_DIR}/resources/icons.h
        ${CMAKE_BINARY_DIR}/resources/resources.c
    COMMAND
        go run ${CMAKE_CURRENT_LIST_DIR}/bundle-resources.go
        ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/bundle-resources.go ${ICONS}
)
add_custom_target(
    Bundle_resources
    DEPENDS
        ${CMAKE_BINARY_DIR}/resources/icons.h
        ${CMAKE_BINARY_DIR}/resources/resources.c
)

add_dependencies(WirelessMouseDesktop Bundle_resources)
include_directories(${CMAKE_BINARY_DIR}/resources)
target_sources(
    WirelessMouseDesktop
    PRIVATE ${CMAKE_BINARY_DIR}/resources/resources.c
)

set(WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/favicon.rc)
target_sources(WirelessMouseDesktop PRIVATE ${WIN32_RESOURCES})
