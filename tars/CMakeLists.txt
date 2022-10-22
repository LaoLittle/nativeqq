cmake_minimum_required(VERSION 3.23)
project (nativeqq_tars)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -lpthread")

aux_source_directory(src DIR_SRCS)

add_library(${PROJECT_NAME} ${DIR_SRCS})
add_library(nativeqq::tars ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} nativeqq::protocol)

target_link_options(${PROJECT_NAME} PUBLIC -static)

