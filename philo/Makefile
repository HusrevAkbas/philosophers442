CFLAGS = -Wall -Werror -Wextra -g
CC = cc

SRC = philo.c errors.c argument_check.c utils.c routine.c init_philos.c
OBJ = $(SRC:.c=.o)

NAME = philo

all : $(NAME)
#	./$(NAME) 3 803 500 450
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
	valgrind --tool=memcheck ./$(NAME) 32 610 200 100 32

hel :
	valgrind --tool=helgrind ./$(NAME) 32 300 100 100 32

.phony : OBJ clean c re fclean f all val