cmake_minimum_required(VERSION 3.15)

project(ssd-maintainer)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCFILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRCFILES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
