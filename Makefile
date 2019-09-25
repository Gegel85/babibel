CLIENT_NAME = babel_client
SERVER_NAME = babel_server

NAME = $(CLIENT_NAME) $(SERVER_NAME)

CMAKE_FLAGS =

BUILD_FOLDER = build_folder

all:	$(NAME)

$(CLIENT_NAME): $(BUILD_FOLDER) $(BUILD_FOLDER)/$(CLIENT_NAME)
	cp $(BUILD_FOLDER)/$(CLIENT_NAME) .

$(SERVER_NAME): $(BUILD_FOLDER) $(BUILD_FOLDER)/$(SERVER_NAME)
	cp $(BUILD_FOLDER)/$(SERVER_NAME) .

$(BUILD_FOLDER)/$(CLIENT_NAME):
	cd $(BUILD_FOLDER) && cmake --build .

$(BUILD_FOLDER)/$(SERVER_NAME):
	cd $(BUILD_FOLDER) && cmake --build .

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
