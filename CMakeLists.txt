cmake_minimum_required(VERSION 3.12)

project(bulk)

set (CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


IF (WIN32)
    set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
ENDIF()


add_library(async SHARED
    src/libasync/async.cpp
    src/libasync/cmd.cpp
    src/libasync/utils.cpp
    src/libasync/writer_thread.cpp
    src/libasync/writers.cpp
    src/libasync/observer.h
    src/libasync/stdafx.h
)

add_executable(bulk
    src/bulk/main.cpp
)

target_include_directories(bulk PRIVATE
    ${CMAKE_BINARY_DIR}/../src/bulk
    ${CMAKE_BINARY_DIR}/../src/libasync
)

target_link_libraries(bulk PRIVATE Threads::Threads async)

install(TARGETS bulk RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_FILE_NAME "bulk")
set(CPACK_PACKAGE_CONTACT art.shestakov@icloud.com)
include(CPack)
