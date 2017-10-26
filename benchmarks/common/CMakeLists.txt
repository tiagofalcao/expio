get_filename_component(TEST ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB SOURCES "*.cc")

add_executable(${TEST} ${SOURCES})
target_link_libraries(${TEST} "${CMAKE_PROJECT_NAME}" ${benchmark_LIBRARIES} Threads::Threads)
#target_include_directories(${TEST} SYSTEM ${benchmark_INCLUDE_DIRS})
