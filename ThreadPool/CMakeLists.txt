file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(MyThreadPool ${SRC_FILES})
target_link_libraries(MyThreadPool pthread)