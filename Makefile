SRCS			= pipex.c pipex_tools.c pipex_tools_2.c main.c
OBJS			= $(SRCS:.c=.o)
CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror
NAME			= pipex

all:			$(NAME)

$(NAME):		$(OBJS)
				ar -rs $(NAME) $(OBJS)

%.o : %.c pipex.h
	$(CC) $(CFLAGS) -c $< -o $(NAME)

clean:
				$(RM) $(OBJS) $(OUTPUT)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all
