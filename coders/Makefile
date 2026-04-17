NAME=codexion
CC=cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC=parsing.c main.c init.c
OBJ=$(SRC:%.c=%.o)
INC=coders/codexion.h

all:$(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c $(INC)
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	@rm -f $(OBJ)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : clean
