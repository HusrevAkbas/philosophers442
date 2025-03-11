CFLAGS = -Wall -Werror -Wextra
CC = cc

SRC = philo.c errors.c utils.c
OBJ = $(SRC:.c=.o)

NAME = philo

all : $(NAME) clean
	./$(NAME) 12 122 -2 -1

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

.phony : OBJ clean c re fclean f all