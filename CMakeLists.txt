cmake_minimum_required(VERSION 3.0)
project(websocket_server)

find_package(Boost)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-D BOOST_LOG_DYN_LINK
        -D BOOST_USE_LIB
        -D BOOST_ALL_DYN_LINK
        -std=c++14
        -DAPI_EXPORTS)


set(WERROR_FLAGS
        -W
        -Wall
        -Wmissing-declarations
        -Wpointer-arith
        -Wcast-align
        -Wcast-qual
        -Wformat-nonliteral
        -Wformat-security
        -Wundef
        -Wwrite-strings
        -Wdeprecated)

#set (CMAKE_EXE_LINKER_FLAGS "-Xlinker -Map=xdx.map")

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CFLAGS
            -pthread
            -march=native
            -g
            ${WERROR_FLAGS})
ELSE ()
    set(CFLAGS
            -pthread
            -march=native
            -O3
            -DNDEBUG
            ${WERROR_FLAGS})
ENDIF ()


add_definitions(-D BOOST_LOG_DYN_LINK
        -D BOOST_USE_LIB
        -D BOOST_ALL_DYN_LINK)
add_definitions(-DRTE_MACHINE_CPUFLAG_SSE
        -DRTE_MACHINE_CPUFLAG_SSE2
        -DRTE_MACHINE_CPUFLAG_SSE3
        -DRTE_MACHINE_CPUFLAG_SSSE3
        -DRTE_MACHINE_CPUFLAG_SSE4_1
        -DRTE_MACHINE_CPUFLAG_SSE4_2
        -DRTE_MACHINE_CPUFLAG_AES
        -DRTE_MACHINE_CPUFLAG_PCLMULQDQ
        -DRTE_MACHINE_CPUFLAG_AVX
        -DRTE_MACHINE_CPUFLAG_RDRAND
        -DRTE_MACHINE_CPUFLAG_FSGSBASE
        -DRTE_MACHINE_CPUFLAG_F16C
        -DRTE_MACHINE_CPUFLAG_AVX2
        -D_GNU_SOURCE)

add_compile_options(${CFLAGS})


include_directories(
        ./
        /usr/local/include/boost
        /usr/include/aarch64-linux-gnu/
        /usr/include
        /usr/include/mysql/
        /usr/local/cuda-10.2/targets/aarch64-linux/include
        /usr/local/cuda-10.2/include
)

link_directories(
        /usr/local/lib
        /usr/lib/
        /usr/local/cuda-10.2/targets/aarch64-linux/lib/
        /usr/lib/aarch64-linux-gnu/
        /usr/local/cuda-10.2/lib64
)
file(GLOB_RECURSE CURRENT_SOURCES
        ${PROJECT_SOURCE_DIR}/websocket/*.cpp

        main.cpp)

add_executable(${PROJECT_NAME}
        ${CURRENT_SOURCES})

target_link_libraries(${PROJECT_NAME}
        nvinfer
        cudart
        curand
        nvonnxparser
        cufft
        cublas
        pthread
        boost_system
        boost_filesystem
        boost_chrono
        boost_date_time
        boost_thread
        boost_log_setup
        boost_locale
        boost_log
        boost_program_options
        boost_serialization
        pthread
        pthread rt
        dl m
        cuda
        stdc++fs  #解决日志std::experimental::filesystem 相关错误
)
