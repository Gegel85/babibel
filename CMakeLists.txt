cmake_minimum_required(VERSION 3.14)
project(CPP_babel_2019)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(
	babel_client
	src/main.cpp
)

target_link_libraries(
	babel_client
	${CONAN_LIBS}
	Qt5::Widgets
)

add_executable(
	babel_server
	src/main.cpp
)

target_link_libraries(
	babel_server
	${CONAN_LIBS}
	Qt5::Widgets
)