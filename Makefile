# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/02 12:06:33 by nlaurids          #+#    #+#              #
#    Updated: 2021/12/02 12:06:33 by nlaurids         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ░░░░▄▄▄▄▀▀▀▀▀▀▀▀▄▄▄▄▄▄
# ░░░░█░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░▀▀▄
# ░░░█░░░▒▒▒▒▒▒░░░░░░░░▒▒▒░░█
# ░░█░░░░░░▄██▀▄▄░░░░░▄▄▄░░░█
# ░▀▒▄▄▄▒░█▀▀▀▀▄▄█░░░██▄▄█░░░█
# █▒█▒▄░▀▄▄▄▀░░░░░░░░█░░░▒▒▒▒▒█
# █▒█░█▀▄▄░░░░░█▀░░░░▀▄░░▄▀▀▀▄▒█
# ░█▀▄░█▄░█▀▄▄░▀░▀▀░▄▄▀░░░░█░░█
# ░░█░░▀▄▀█▄▄░█▀▀▀▄▄▄▄▀▀█▀██░█
# ░░░█░░██░░▀█▄▄▄█▄▄█▄████░█
# ░░░░█░░░▀▀▄░█░░░█░███████░█
# ░░░░░▀▄░░░▀▀▄▄▄█▄█▄█▄█▄▀░░█
# ░░░░░░░▀▄▄░▒▒▒▒░░░░░░░░░░█
# ░░░░░░░░░░▀▀▄▄░▒▒▒▒▒▒▒▒▒▒░█
# ░░░░░░░░░░░░░░▀▄▄▄▄▄░░░░░█

NAME		=	$(shell basename $(CURDIR))

CXX			=	clang++

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic -g -fsanitize=address

SRCS		=	$(wildcard src/*.cpp)

OBJS		=	$(SRCS:.cpp=.o)

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean	:
			rm -f $(OBJS)

fclean	:	clean
			rm -f $(NAME) all_data/uploads/*

re		:	fclean all