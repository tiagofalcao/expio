cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

find_program (CLANG_TIDY NAMES "clang-tidy")
if(CLANG_TIDY)
	set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif()

project(expio C CXX)

if(${CMAKE_VERSION} VERSION_GREATER "3.1.0") 
	cmake_policy(SET CMP0054 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER "3.3.0") 
	cmake_policy(SET CMP0057 NEW)
endif()

include (GNUInstallDirs)

find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -Wextra -Werror")
     set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
     set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

find_package(benchmark)
if(BENCHMARK_FOUND)
    include_directories(SYSTEM ${benchmark_INCLUDE_DIRS})
    add_subdirectory(benchmarks)
endif()

