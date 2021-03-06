cmake_minimum_required(VERSION 3.12)
project(CPP_babel_2019)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		add_compile_options(-g -O0)
	endif()
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options(-W -Wall -Wextra)
endif ()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if (WIN32)
	if (Qt5_PLUGINS_DIR)
		file(TO_CMAKE_PATH "${Qt5_PLUGINS_DIR}" Qt5_PLUGINS_DIR)
		file(WRITE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qt.conf [Paths]\nPlugins=${Qt5_PLUGINS_DIR})
	else ()
		message(FATAL_ERROR "Please define Qt5_PLUGINS_DIR")
	endif()
endif ()

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
	src/client/graphical/qt5/BabelQTClient.hpp
	src/client/graphical/qt5/BabelQTClient.cpp
	src/client/graphical/qt5/QTButton.hpp
	src/client/graphical/qt5/QTButton.cpp
	src/client/graphical/qt5/QTApplication.hpp
	src/client/graphical/qt5/QTApplication.cpp
	src/client/graphical/qt5/QTScrollBar.cpp
	src/client/graphical/qt5/QTScrollBar.hpp
	src/client/graphical/qt5/QTTypingBox.cpp
	src/client/graphical/qt5/QTTypingBox.hpp
	src/client/graphical/qt5/QTTextBox.cpp
	src/client/graphical/qt5/QTTextBox.hpp
	src/client/graphical/qt5/QTLayout.cpp
	src/client/graphical/qt5/QTLayout.hpp
	src/client/graphical/GUIScreen.hpp
	src/client/graphical/GUIScreen.cpp
	src/client/sound/Compressor.cpp
	src/client/sound/Compressor.hpp
	src/client/sound/CompressedAudio.cpp
	src/client/sound/CompressedAudio.hpp
	src/client/sound/Sound.cpp
	src/client/sound/Sound.hpp
	src/client/main.cpp
	src/network/Socket.cpp
	src/network/Socket.hpp
	src/network/Protocol.cpp
	src/network/Protocol.hpp
	src/network/SocketExceptions.hpp
	src/network/ServerSocket.cpp
	src/network/ServerSocket.hpp
	src/client/TcpClient.cpp
	src/client/TcpClient.hpp
	src/client/sound/Compressor.cpp
	src/client/sound/Compressor.hpp
)

if (WIN32)
	target_link_libraries(
		babel_client
		${CONAN_LIBS}
		Qt5::Widgets
		ws2_32
	)
else ()
	target_link_libraries(
		babel_client
		${CONAN_LIBS}
		Qt5::Widgets
	)
endif ()

add_executable(
	babel_server
	src/server/main.cpp
	src/network/Socket.cpp
	src/network/Socket.hpp
	src/network/Protocol.cpp
	src/network/Protocol.hpp
	src/network/SocketExceptions.hpp
	src/network/ServerSocket.cpp
	src/network/ServerSocket.hpp
	src/server/TcpServer.cpp
	src/server/TcpServer.hpp
	src/server/Database.cpp
	src/server/Database.hpp
)

if (WIN32)
	target_link_libraries(
		babel_server
		${CONAN_LIBS}
		ws2_32
	)
else ()
	target_link_libraries(
		babel_server
		${CONAN_LIBS}
	)
endif ()
