cmake_minimum_required(VERSION 3.12)

project(bulk_server)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

IF (WIN32)
    set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
ENDIF()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})

    add_executable(bulk_server
        src/bulk/main.cpp
        src/bulk/network.cpp
        src/bulk/server.cpp
        src/bulk/session.cpp
    )

    add_library(async SHARED
        src/libasync/async.cpp
        src/libasync/cmd.cpp
        src/libasync/utils.cpp
        src/libasync/writers.cpp
        src/libasync/writer_thread.cpp
        src/libasync/observer.h
        src/libasync/stdafx.h
    )

    target_link_libraries(bulk_server ${Boost_LIBRARIES})

    if(WIN32)
        add_definitions(
            -D_WIN32_WINDOWS
            -D_WINSOCK_DEPRECATED_NO_WARNINGS
        )
    endif()
endif()

target_include_directories(bulk_server
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(bulk_server PRIVATE
    ${CMAKE_BINARY_DIR}/../src/bulk
    ${CMAKE_BINARY_DIR}/../src/libasync
)

target_link_libraries(bulk_server PRIVATE Threads::Threads async)

install(TARGETS bulk_server RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_FILE_NAME "bulk_server")
set(CPACK_PACKAGE_CONTACT art.shestakov@icloud.com)
include(CPack)
