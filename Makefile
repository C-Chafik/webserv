NAME=WebServer
CC=c++
FLAGS=-Wall -Werror -Wextra
SRC=server.cpp request_handler.cpp accept_connection.cpp handle_request.cpp
SRCS=$(addprefix srcs/, $(SRC))
INC_DIR=includes
OBJS=${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJS): $(SRCS)
	$(CC) -I$(INC_DIR) $(SRCS) -c

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:all, re, clean, fclean