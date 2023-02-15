#ifndef PHILO3_H
# define PHILO3_H

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
	int				between_eating;
	//int				philo_dead;
}					t_table;

typedef struct s_philo
{
	int				death;
	int				eat;
	int				sleep;
	int				limit;
	int				time;
	int				philo_dead;
	pthread_mutex_t	pl;
	pthread_mutex_t	pl2;
	int				count;
	t_table			*place;
}					t_philo;

#endif