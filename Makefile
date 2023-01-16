# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/14 12:33:34 by lomasson          #+#    #+#              #
#    Updated: 2023/01/16 12:29:52 by lomasson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_CLIENT = client

NAME_SERVER = server

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3

SRCS_CLIENT = client.cpp

SRCS_SERVER = server.cpp

OBJS_CLIENT = $(SRCS_CLIENT:%.cpp=%.o)

OBJS_SERVER = $(SRCS_SERVER:%.cpp=%.o)

.PHONY: all

all: $(NAME_CLIENT) $(NAME_SERVER)

%.o: %.cpp
		$(CC) -c $(CFLAGS) $< -o $@

$(NAME_CLIENT): $(OBJS_CLIENT)
		$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $@
		@printf "\n\033[0;32mExecutable name: ./$(NAME_CLIENT)\033[0;32m\n"
		
$(NAME_SERVER): $(OBJS_SERVER)
		$(CC) $(CFLAGS) $(OBJS_SERVER) -o $@
		@printf "\n\033[0;32mExecutable name: ./$(NAME_SERVER)\033[0;32m\n"
		
.PHONY: clean fclean re

clean:
		rm -f $(OBJS_CLIENT)
		rm -f $(OBJS_SERVER)

fclean: clean
		rm -f $(NAME_CLIENT)
		rm -f $(NAME_SERVER)

re: fclean all