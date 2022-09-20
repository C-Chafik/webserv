NAME=WebServer
CC=c++
FLAGS= -Wall -Werror -Wextra -std=c++98

SRC= main.cpp server.cpp request_handler.cpp accept_connection.cpp \
	handle_request.cpp fileLocation.cpp parseConfig.cpp host.cpp \
	send.cpp utils.cpp listen.cpp parseConfig_insert.cpp \
  parseConfig_utils.cpp parseConfig_common.cpp
SRCS=$(addprefix srcs/, $(SRC))
INC_DIR=includes
OBJS=${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJS): $(SRCS)
	$(CC)  $(FLAGS) -I $(INC_DIR) $(SRCS) -c

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY:all, re, clean, fclean
