NAME	= philo
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread
RM		= rm -f

SRCS	= main.c \
		  init.c \
		  routine.c \
		  utils.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
