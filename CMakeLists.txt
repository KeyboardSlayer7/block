cmake_minimum_required(VERSION 3.0.0)

project(block VERSION 0.0.1 LANGUAGES C CXX)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES 
	${SOURCE_DIR}/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# target_include_directories(${PROJECT_NAME} PUBLIC {{INCLUDE_DIRECTORIES}})

# target_link_directories(${PROJECT_NAME} PUBLIC {{LINK_DIRECTORIES}})

target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)