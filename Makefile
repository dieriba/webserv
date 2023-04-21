SRCS_MAIN	=	main.cpp

SRCS_		=	./srcs/TcpServer.cpp \
				./srcs/Server.cpp \
				./srcs/ExceptionThrower.cpp \
				./srcs/Location.cpp \
				./srcs/StringUtils.cpp \
				./srcs/Parser.cpp \
				./srcs/BitsManipulation.cpp \

SRCS_IO		=	./srcs/IO/IO.cpp \
				./srcs/IO/ServerStream.cpp \
				./srcs/IO/ClientSocketStream.cpp \
				./srcs/IO/CgiStream.cpp \

OBJS	=	$(SRCS_MAIN:.cpp=.o) \
			$(SRCS_:.cpp=.o) \
			$(SRCS_IO:.cpp=.o)

DEPS 	=	$(SRCS_MAIN:.cpp=.d) \
			$(SRCS_:.cpp=.d) \
			$(SRCS_IO:.cpp=.d)

CC		=	c++ -g3

CFLAGS	=	-Wall -Wextra -Werror -MMD -std=c++98

NAME	=	webserv

.cpp.o :	
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME):	$(OBJS)	
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all:		$(NAME)

clean:
			rm -rf $(OBJS)
			rm -rf $(DEPS)
			
fclean:		clean
			rm -rf $(NAME)

re:			fclean all

-include : $(DEPS)

.PHONY:		all clean fclean re