CLIENT_NAME = babel_client
SERVER_NAME = babel_server

NAME = $(CLIENT_NAME) $(SERVER_NAME)

SSRC =	./src/server/TcpServer.cpp \
	./src/server/main.cpp \
	./src/network/Protocol.cpp \
	./src/network/Socket.cpp \
	./src/network/ServerSocket.cpp \

CSRC =	./src/client/graphical/GUIScreen.cpp \
	./src/client/graphical/qt5/QTApplication.cpp \
	./src/client/graphical/qt5/QTWindow.cpp \
	./src/client/graphical/qt5/QTButton.cpp \
	./src/client/graphical/qt5/myQTWindow.cpp \
	./src/client/graphical/qt5/QTScrollBar.cpp \
	./src/client/graphical/Widget/ScrollBar.cpp \
	./src/client/graphical/Widget/ScrollBar.hpp \
	./src/client/graphical/Widget/Widget.cpp \
	./src/client/graphical/Widget/Button.cpp \
	./src/client/graphical/Widget/Picture.cpp \
	./src/client/graphical/Widget/TypingBox.cpp \
	./src/client/graphical/Widget/TextBox.cpp \
	./src/client/main.cpp \
	./src/network/Protocol.cpp \
	./src/network/Socket.cpp \
	./src/network/ServerSocket.cpp \

OBJ = $(CSRC:%.cpp=%.o) $(SSRC:%.cpp=%.o)

CMAKE_FLAGS =

BUILD_FOLDER = build_folder

CXX = g++

CXXFLAGS = -W -Wall -Wextra -g -O0

LDFLAGS = -lpthread

all:	$(NAME)

mano:	$(OBJ)
	$(CXX) -o $(CLIENT_NAME) $(CSRC:%.cpp=%.o) $(LDFLAGS)
	$(CXX) -o $(SERVER_NAME) $(SSRC:%.cpp=%.o) $(LDFLAGS)

$(CLIENT_NAME): $(BUILD_FOLDER) $(BUILD_FOLDER)/$(CLIENT_NAME)
	cp $(BUILD_FOLDER)/bin/$(CLIENT_NAME) .

$(SERVER_NAME): $(BUILD_FOLDER) $(BUILD_FOLDER)/$(SERVER_NAME)
	cp $(BUILD_FOLDER)/bin/$(SERVER_NAME) .

$(BUILD_FOLDER)/%:
	cd $(BUILD_FOLDER) && cmake --build . --target `echo $@ | sed -e "s/$(BUILD_FOLDER)\///"`

$(BUILD_FOLDER):
	mkdir $(BUILD_FOLDER) && cd $(BUILD_FOLDER) && (conan install .. || conan install .. --build) && cmake .. $(CMAKE_FLAGS)

add_remotes:
	conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
	conan remote add epitech https://api.bintray.com/conan/epitech/public-conan
	conan remote add conan https://api.bintray.com/conan/conan/conan-center

clean:
	rm -rf $(BUILD_FOLDER)

fclean: clean
	rm -rf $(NAME)

re:	fclean all

dbg:	fclean
dbg:	CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Debug
dbg:	all

.PHONY: all re clean fclean add_remotes

