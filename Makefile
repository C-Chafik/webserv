NAME=WebServer
CC=c++
FLAGS=-Wall -Werror -Wextra -std=c++98 -g -O0

SRC=main.cpp server.cpp request_handler.cpp accept_connection.cpp handle_request.cpp fileLocation.cpp parseConfig.cpp host.cpp send.cpp utils.cpp
SRCS=$(addprefix srcs/, $(SRC))
INC_DIR=includes
LIB=-pthread
OBJS=${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LIB)  $(OBJS) -o $(NAME)

$(OBJS): $(SRCS)
	$(CC) -I$(INC_DIR) $(SRCS) -c

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY:all, re, clean, fclean
