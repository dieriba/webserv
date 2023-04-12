MAIN	=	main.cpp
		
SRCS	=	./srcs/TcpServer.cpp \
			./srcs/Server.cpp \
			./srcs/ExceptionThrower.cpp \
			./srcs/Location.cpp \
			
OBJS	=	$(MAIN:.cpp=.o)\
			$(SRCS:.cpp=.o)

DEPS 	=	$(MAIN:.cpp=.d)\
			$(SRCS:.cpp=.o)

CC		=	c++ -g3

CFLAGS	=	-Wall -Wextra -Werror -MMD -std=c++98

NAME	=	webserv

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