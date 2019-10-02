cmake_minimum_required(VERSION 3.14)
project(CPP_babel_2019)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_compile_options(-g -O0)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
conan_basic_setup()

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
	src/client/graphical/qt5/QTCursors.hpp
	src/client/graphical/qt5/QTWindow.hpp
	src/client/graphical/qt5/QTWindow.cpp
	src/client/graphical/qt5/myQTWindow.hpp
	src/client/graphical/qt5/myQTWindow.cpp
	src/client/graphical/qt5/QTButton.hpp
	src/client/graphical/qt5/QTButton.cpp
	src/client/graphical/qt5/QTApplication.hpp
	src/client/graphical/qt5/QTApplication.cpp
	src/client/graphical/qt5/QTScrollBar.cpp
	src/client/graphical/qt5/QTScrollBar.hpp
	src/client/graphical/qt5/QTTextBox.cpp
	src/client/graphical/qt5/QTTextBox.hpp
	src/client/graphical/GUIScreen.hpp
	src/client/graphical/GUIScreen.cpp
	src/client/main.cpp
	src/network/Protocol.cpp
	src/network/Protocol.hpp
)

target_link_libraries(
  babel_client
  ${CONAN_LIBS}
  Qt5::Widgets
)

add_executable(
	babel_server
	src/server/main.cpp
	src/network/Socket.cpp
	src/network/Socket.hpp
	src/network/Protocol.cpp
	src/network/Protocol.hpp
	src/network/SocketExceptions.hpp src/network/ServerSocket.cpp src/network/ServerSocket.hpp src/server/TcpServer.cpp src/server/TcpServer.hpp)

target_link_libraries(
	babel_server
	${CONAN_LIBS}
)
