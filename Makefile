NAME = webserv
CXX = c++
# CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -O2 -MMD -MP
CXXFLAGS = -Wall -Wextra -std=c++98 -O2 -MMD -MP
SRC =	src/main.cpp\
		src/Parser.cpp\
		src/Server.cpp
INC = -Iinclude
OBJ = $(patsubst src/%.cpp,obj/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $(SRC) -o $(NAME)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ 

clean:
	rm -rf obj
	rm -rf $(NAME).d

fclean: clean
	rm -rf $(NAME)

re: fclean all

-include $(OBJ:.o=.d)
