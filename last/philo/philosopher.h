/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:41:37 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/04 13:07:52 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include "fcntl.h"
# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "unistd.h"
# include <pthread.h>
# include <sys/time.h>

typedef struct s_fork
{
	int				left;
	int				right;
}					t_fork;

typedef struct s_table
{
	int				philo;
	pthread_mutex_t	fork;
	int				fork_lock;
	struct s_table	*right;
	struct s_table	*left;
	int				eaten;
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	lock1;
	pthread_mutex_t	lock2;
	pthread_mutex_t	lock3;
	time_t			between_eating;
	int				philo_dead;
	time_t			death_limit;
	time_t			eat_time;
	time_t			sleep_time;
	time_t			eat_limiter;
	time_t			time;
	struct s_table	*first;
	int				philo_count;
	int				i;
	int				j;
	int				k;
	struct timeval	b_eat;
}					t_table;

int					ft_atoi(const char *str);
int					philo_eating(t_table *p);
int					philo_sleeping(t_table *p);
int					get_time(void);
void				eat_timer(time_t time);
void				*philo(void *ph);
int					only_one_philo(t_table *p);
int					check_if_filled(t_table *p, int limit);
void				philo2(void *ph, int i);

#endif