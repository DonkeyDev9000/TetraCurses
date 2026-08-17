cmake_minimum_required(VERSION 3.28)

set(TARGET_NAME tetra-curses)
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 23)

find_package(Curses REQUIRED)

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
)

add_subdirectory(extern/TetraCore SYSTEM)

add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE
        "src/"
)

target_link_libraries(${TARGET_NAME}
        PRIVATE
        ${CURSES_LIBRARIES}
        tetra-core
)
