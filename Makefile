SRCS_MAIN   = main.cpp

SRCS_SERVER	= ./srcs/server/HttpServer.cpp \
              ./srcs/server/Server.cpp \
              ./srcs/server/Location.cpp \
              
SRCS_UTILS	= ./srcs/utils/ExceptionThrower.cpp \
              ./srcs/utils/UtilityMethod.cpp \
              ./srcs/utils/Parser.cpp \
              ./srcs/utils/BitsManipulation.cpp \

SRCS_IO     = ./srcs/IO/IO.cpp \
              ./srcs/IO/ServerStream.cpp \
              ./srcs/IO/ClientSocketStream.cpp \
              ./srcs/IO/CgiStream.cpp \

SRCS_HTTP   = ./srcs/http/HttpMessage.cpp \
              ./srcs/http/HttpRequest.cpp \
              ./srcs/http/HttpResponse.cpp \
              ./srcs/http/RequestChecker.cpp \

SRCS_METHOD	= ./srcs/method/Method.cpp \
			  ./srcs/method/Get.cpp \
			  ./srcs/method/Post.cpp \
			  ./srcs/method/Delete.cpp \
			  ./srcs/method/Error.cpp \

OBJ_DIR     = ./obj
DEP_DIR     = ./dep

OBJS        = $(addprefix $(OBJ_DIR)/, $(SRCS_MAIN:.cpp=.o)) \
              $(addprefix $(OBJ_DIR)/, $(SRCS_SERVER:.cpp=.o)) \
              $(addprefix $(OBJ_DIR)/, $(SRCS_UTILS:.cpp=.o)) \
              $(addprefix $(OBJ_DIR)/, $(SRCS_IO:.cpp=.o)) \
              $(addprefix $(OBJ_DIR)/, $(SRCS_HTTP:.cpp=.o)) \
              $(addprefix $(OBJ_DIR)/, $(SRCS_METHOD:.cpp=.o)) \

DEPS        = $(addprefix $(DEP_DIR)/, $(SRCS_MAIN:.cpp=.d)) \
              $(addprefix $(DEP_DIR)/, $(SRCS_SERVER:.cpp=.d)) \
              $(addprefix $(DEP_DIR)/, $(SRCS_UTILS:.cpp=.d)) \
              $(addprefix $(DEP_DIR)/, $(SRCS_IO:.cpp=.d)) \
              $(addprefix $(DEP_DIR)/, $(SRCS_HTTP:.cpp=.d)) \
              $(addprefix $(DEP_DIR)/, $(SRCS_METHOD:.cpp=.d)) \

CC          = c++ -g3

CFLAGS      = -Wall -Wextra -Werror -MMD -std=c++98

NAME        = webserv

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(OBJ_DIR) $(DEP_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re