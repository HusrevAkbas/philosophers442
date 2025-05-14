CFLAGS = -Wall -Werror -Wextra -g
CC = cc

SRC = philo.c errors.c argument_check.c utils.c routine.c init_philos.c
OBJ = $(SRC:.c=.o)

NAME = philo

all : $(NAME)

$(NAME) : $(OBJ) philo.h
	$(CC) $(CFLAGS) $(OBJ) -lpthread -o $(NAME)

$(OBJ) : $(SRC) philo.h
	$(CC) $(CFLAGS) -c $(SRC)

re : fclean all

c : clean
clean :
	rm -fr ${OBJ}

f : fclean
fclean : clean
	rm -fr $(NAME)

val :
	valgrind --tool=memcheck  --leak-check=full --show-leak-kinds=all ./$(NAME) 5 810 200 200 10

hel :
	valgrind --tool=helgrind --history-level=none ./$(NAME) 54 800 200 190 32

.phony : clean c re fclean f all val hel