NAME=codexion
CC=cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC=parsing.c main.c init_part_1.c init_part_2.c queue.c utils.c threads.c
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
	@echo "\033[0;31m✗ Removing object files...\033[0m"
	@rm -f $(OBJ)
	@echo "\033[1;32m✓ Object files successfully removed!\033[0m"

fclean : clean
	@echo "\033[0;31m✗ Removing $(NAME)...\033[0m"
	@rm -f $(NAME)
	@echo "\033[1;32m✓ $(NAME) successfully removed!\033[0m"

re : fclean all

.PHONY : clean