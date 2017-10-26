get_filename_component(TEST ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB SOURCES "*.cc")

add_executable(${TEST} ${SOURCES})
target_link_libraries(${TEST} "${CMAKE_PROJECT_NAME}" pthread)
add_test(NAME ${TEST} COMMAND ${TEST})
set_tests_properties(${${TEST}_tests} PROPERTIES TIMEOUT 10)
