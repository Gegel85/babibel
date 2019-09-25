cmake_minimum_required(VERSION 3.14)
project(CPP_babel_2019)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(
	babel_client
	src/client/data/Vector2.hpp
	src/client/graphical/Widget/TextBox.cpp
	src/client/graphical/Widget/Picture.hpp
	src/client/graphical/Widget/Widget.hpp
	src/client/graphical/Widget/Widget.cpp
	src/client/graphical/Widget/TypingBox.cpp
	src/client/graphical/Widget/Button.hpp
	src/client/graphical/Widget/ScrollBar.hpp
	src/client/graphical/Widget/Button.cpp
	src/client/graphical/Widget/ScrollBar.cpp
	src/client/graphical/Widget/Picture.cpp
	src/client/graphical/Widget/TextBox.hpp
	src/client/graphical/Widget/TypingBox.hpp
	src/client/graphical/qt5/QTWindow.cpp
	src/client/graphical/qt5/QTCursors.hpp
	src/client/graphical/qt5/QTWindow.hpp
	src/client/graphical/qt5/QTButton.hpp
	src/client/graphical/qt5/QTButton.cpp
	src/client/graphical/GUIScreen.hpp
	src/client/graphical/GUIScreen.cpp
	src/client/main.cpp
)

target_link_libraries(
	babel_client
	${CONAN_LIBS}
	Qt5::Widgets
)

add_executable(
	babel_server
	src/server/main.cpp
	src/server/TcpServer.cpp
	src/server/TcpServer.hpp
	src/server/TcpConnection.cpp
	src/server/TcpConnection.hpp
)

target_link_libraries(
	babel_server
	${CONAN_LIBS}
	Qt5::Widgets
)
