CFLAGS = -Wall -Werror -Wextra -g
CC = cc

SRC = philo.c errors.c argument_check.c utils.c
OBJ = $(SRC:.c=.o)

NAME = philo

all : $(NAME) clean
	./$(NAME) 100 606 300 100 10
# check the issue when max food is 1 solved with additional mutex for shared pointer
$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lpthread -o $(NAME)

$(OBJ) : $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

re : fclean all

c : clean
clean :
	rm -f *.o

f : fclean
fclean : clean
	rm -f $(NAME)

val :
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all ./$(NAME) 2 600 300 400

.phony : OBJ clean c re fclean f all val