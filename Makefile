SRCS_MAIN	=	main.cpp

SRCS_		=	./srcs/TcpServer.cpp \
				./srcs/Server.cpp \
				./srcs/ExceptionThrower.cpp \
				./srcs/Location.cpp

OBJS	=	$(SRCS_MAIN:.cpp=.o)\
			$(SRCS_:.cpp=.o)

DEPS 	=	$(SRCS_MAIN:.cpp=.d)\
			$(SRCS_:.cpp=.d)

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