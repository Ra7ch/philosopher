# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/03 23:11:55 by raitmous          #+#    #+#              #
#    Updated: 2023/03/03 23:28:44 by raitmous         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= main_bonus.c philo_bonus_utils.c philo_bonus.c philo2_bonus.c
OBJS	= ${SRCS:.c=.o}
CFLAGS	= -Wall -Wextra -Werror
RM		= rm -rf
NAME	= philo

all	: ${NAME}

${NAME} : ${OBJS}
	cc ${CFLAGS} ${SRCS} -o ${NAME}

bonus : ${OBJS}
	cc ${CFLAGS} ${SRCS} -o ${NAME}

thread : ${NAME}
	cc ${CFLAGS} -fsanitize=thread -g  ${SRCS} -o ${NAME}

clean :
	${RM} ${OBJS}

fclean : clean
	${RM} ${NAME}

re : fclean all

a : all clean

b : bonus clean