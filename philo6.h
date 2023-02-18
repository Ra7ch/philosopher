#ifndef PHILO6_H
# define PHILO6_H

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
}					t_table;

#endif