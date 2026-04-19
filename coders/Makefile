NAME=codexion
CC=cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC=parsing.c main.c init.c queue.c utils.c
OBJ=$(SRC:%.c=%.o)
INC=codexion.h headers.h

all:$(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[1;32m✓ $(NAME) successfully compiled!\033[0m"

%.o : %.c $(INC)
	@echo "\033[0;34m⟳ Compiling $<...\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	@rm -f $(OBJ)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : clean
