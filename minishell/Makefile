# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtollena <jtollena@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/30 13:47:45 by jtollena          #+#    #+#              #
#    Updated: 2024/01/23 13:53:23 by jtollena         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = srcs/minishell.c

OBJS = $(SRCS:.c=.o)
INCLUDES = includes
FLAGS = #-Wall -Wextra -Werror

%.o: %.c
	gcc $(FLAGS) -g -Ilibft -I ../readline-master/ -I $(INCLUDES) -c $< -o $@

all: $(NAME)

clean:
	rm $(OBJS)
	
$(NAME): $(OBJS) $(INCLUDES)
	make -C libft/
	gcc $(FLAGS) -g -Llibft -lft -lreadline -l ../readline-master/ $(OBJS) -o $(NAME)
	make clean

fclean: clean
	rm $(NAME)

re: fclean $(NAME)

.PHONY: re fclean clean all