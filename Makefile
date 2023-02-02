# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/14 12:33:34 by lomasson          #+#    #+#              #
#    Updated: 2023/02/02 10:40:06 by lomasson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLU						= \033[0;34m
GRN						= \033[0;32m
RED						= \033[0;31m
RST						= \033[2K\r
END						= \033[0m
CHECK 				= ✓

SRCS_CLIENT 	= srcs/client.cpp
SRCS_SERVER 	= srcs/server.cpp \
								srcs/Settings.cpp \
								srcs/Config.cpp \
								srcs/Request.cpp \
								srcs/Cgi.cpp 
								

NAME_CLIENT 	= client
NAME_SERVER 	= server

OBJS_DIR			= objs/
OBJS_CLIENT		= $(addprefix $(OBJS_DIR), $(notdir $(SRCS_CLIENT:.cpp=.o)))
OBJS_SERVER		= $(addprefix $(OBJS_DIR), $(notdir $(SRCS_SERVER:.cpp=.o)))

RM						= @rm -rf
CC 						= g++

CFLAGS 				= -Wall -Wextra -std=c++98
CPPFLAGS			= -I./includes
DEBEUG 				= -g3# -fsanitize=address

all: $(NAME_SERVER) $(NAME_CLIENT)

parsing: $(NAME_CLIENT)

objs/%.o: srcs/*%.cpp
		@mkdir -p $(OBJS_DIR)
		@${CC} $(CPPFLAGS) -o $@ -c $< ${CFLAGS} ${DEBEUG}
		@printf	"$(RST) ${BLU}$ [BUILD]${RST} '$<' $(END)"


$(NAME_CLIENT): $(OBJS_CLIENT)
		@$(CC) $(OBJS_CLIENT) -o $@ $(CFLAGS) ${DEBEUG}
		@echo "$(RST) $(GRN)[Compiled] $(END)	./$(NAME_CLIENT)	$(CHECK) "
		
$(NAME_SERVER): $(OBJS_SERVER)
		@$(CC) $(OBJS_SERVER) -o $@ $(CFLAGS) ${DEBEUG}
		@echo "$(RST) $(GRN)[Compiled]$(END)	./$(NAME_SERVER)	$(CHECK)"


clean:
		${RM} $(OBJS_CLIENT) $(OBJS_SERVER)
		@echo "$(GRN) [Clean]$(END)	$(CHECK)"

fclean: clean
		${RM} $(NAME_CLIENT) $(NAME_SERVER) $(OBJS_DIR)
		@echo "$(GRN) [Fclean]$(END)	$(CHECK)"

re: fclean all

.PHONY: all clean fclean re
