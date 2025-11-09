NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -O2 #-g
SRC = src/main.cpp
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

fclean: clean
	rm -rf $(NAME)

re: fclean all
