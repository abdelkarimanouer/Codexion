NAME=codexion
CFLAGS = -Wall -Wextra -Werror -pthread
SRC=coders/dongles.c coders/logging.c coders/parsing.c\
coders/coder_routine.c coders/main.c coders/scheduler.c\
coders/init.c coders/monitor.c coders/threads.c
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