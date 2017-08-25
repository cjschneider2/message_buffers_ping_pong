cmake_minimum_required(VERSION 3.0)


include_directories("${CMAKE_SOURCE_DIR}/src/include")

add_compile_options (
    -g
    -std=gnu11
    -Werror
    -Wfatal-errors
)

# Server program
set ( SERVER_SRC src/bin/server/server_main.c)
set ( SERVER_HDR src/bin/server/server.h )

# Client program
set ( CLIENT_SRC src/bin/client/client_main.c)
set ( CLIENT_HDR src/bin/client/client.h )

# Shared Headers
set ( SHARED_HDR src/include/interface.h )

# Program Definitions
add_executable( msgq_server ${SERVER_SRC} ${SERVER_HDR} ${SHARED_HDR} )

add_executable( msgq_client ${CLIENT_SRC} ${CLIENT_HDR} ${SHARED_HDR} )
