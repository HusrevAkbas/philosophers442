CFLAGS = -Wall -Werror -Wextra -g
CC = cc

SRC = philo.c errors.c argument_check.c utils.c routine.c init_philos.c
OBJ = $(SRC:.c=.o)

NAME = philo_bonus

all : $(NAME) run

run:
	./$(NAME) 3 810 400 400 8

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
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes ./$(NAME) 11 810 400 400 8

hel :
	valgrind --tool=helgrind --history-level=none ./$(NAME) 4 800 200 190 2

.phony : OBJ clean c re fclean f all val