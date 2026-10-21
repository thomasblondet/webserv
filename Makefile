NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -O2 -g
INC = -Iinclude -Iinclude/config -Iinclude/http -Iinclude/network -Iinclude/core -Iinclude/utils
SRC = src/main.cpp \
	src/core/Server.cpp \
	src/network/EventManager.cpp \
	src/network/Listener.cpp \
	src/network/Client.cpp \
	src/config/ServerConfig.cpp \
	src/config/LocationConfig.cpp \
	src/config/parsing.cpp \
	src/http/HTTPParser.cpp \
	src/http/HTTPRequest.cpp \
	src/http/HTTPResponse.cpp \
	src/http/Router.cpp \
	src/http/StaticFileHandler.cpp \
	src/http/ErrorHandler.cpp \
	src/http/PostHandler.cpp \
	src/http/DeleteHandler.cpp \
	src/http/StatusLine.cpp \
	src/http/MIME.cpp \
	src/http/CGIHandler.cpp \
	src/http/DirectoryHandler.cpp \
	src/http/RedirectionHandler.cpp \
	src/utils/Helpers.cpp
OBJ = $(patsubst src/%.cpp,obj/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ 

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

run: $(NAME)
	./$(NAME) conf/webserv.conf

re: fclean all

.PHONY: all clean fclean re run