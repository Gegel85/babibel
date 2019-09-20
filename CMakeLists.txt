cmake_minimum_required(VERSION 3.14)
project(CPP_babel_2019)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(
	CPP_babel_2019
	src/main.cpp
)

target_link_libraries(CPP_babel_2019 ${CONAN_LIBS} Qt5::Widgets)