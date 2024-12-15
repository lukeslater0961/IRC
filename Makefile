PROG_NAME = ircserv
CC = c++
FLAGS =  -g -Werror -Wextra -Wall -std=c++98

SRCS = src/main.cpp \
		src/Client.cpp\
		src/Commands.cpp\
		src/Server.cpp\
		src/Utils.cpp \
		src/Channel.cpp \



OBJS = $(SRCS:.cpp=.o)

#_____Define colors_____
GREEN = \033[0;32m
NC = \033[0m
YELLOW = \033[1;33m
RED = \033[0;31m
PURPLE = \033[0;35m
CYAN = \033[0;36m
#______________________

bold := $(shell tput bold)
notbold := $(shell tput sgr0)

#_____MESSAGES_____
define SUCCESS_MESSAGE
	@printf "\r${GREEN}$(bold)[$@ built successfully!]${NC}\n"
endef

define CLEANING_SUCCESS
	@printf "\r${RED}$(bold)[🧹]${NC}$(bold)IRC cleaned successfully!\n"
endef
#__________________

$(PROG_NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(PROG_NAME)
	@echo -n "\033[2K"
	@$(call SUCCESS_MESSAGE, $(PROG_NAME))

%.o: %.cpp
	@$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@
	@echo -n "\033[2K"

clean:
	@rm -f $(OBJS)
	@echo -n "\033[2K"
	@$(call CLEANING_SUCCESS)

fclean: clean
	@rm -f $(PROG_NAME)

re: fclean all

all: $(PROG_NAME)

.PHONY: all clean fclean re
