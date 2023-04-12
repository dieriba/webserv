SRC		=	main.cpp \
			srcs/Server.cpp \
			srcs/Location.cpp

OBJS	=	$(SRC:.cpp=.o)

DEPS 	=	$(SRC:.cpp=.d)

CC		=	c++ -g3

CFLAGS	=	-Wall -Wextra -Werror -MMD -std=c++98

NAME	=	RPN

.cpp.o :	
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME):	$(OBJS)	
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all:		$(NAME)

clean:
			rm -rf *.o
			rm -rf *.d
			
fclean:		clean
			rm -rf $(NAME)

re:			fclean all

-include : $(DEPS)

.PHONY:		all clean fclean re