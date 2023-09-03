/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:12:34 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/26 21:23:09 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_BONUS_H
# define PHILOSOPHER_BONUS_H

# include "fcntl.h"
# include "stdio.h"
# include "stdlib.h"
# include "string.h"
# include "unistd.h"
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/time.h>

typedef struct s_fork
{
	int				left;
	int				right;
}					t_fork;

typedef struct s_table
{
	int				philo;
	int				fork_lock;
	struct s_table	*right;
	struct s_table	*left;
	int				eaten;
	time_t			between_eating;
	int				*philo_dead;
	time_t			death_limit;
	time_t			eat_time;
	time_t			sleep_time;
	int				eat_limiter;
	time_t			time;
	struct s_table	*first;
	int				philo_count;
	int				fork_count;
	int				i;
	int				j;
	int				k;
	sem_t			*check_dead;
	sem_t			*dead;
	pid_t			pid;
}					t_table;

typedef struct s_sem
{
	sem_t			*ph;
	sem_t			*d;
	sem_t			*wait;
	time_t			time;
}					t_sem;

pid_t				ft_fork(t_table *p, sem_t *ph, sem_t *d, sem_t *wait);
int					ft_atoi(const char *str);
int					get_time(void);
void				philo_eating(t_table *p, sem_t *d, sem_t *ph, sem_t *wait);
void				hold_forks(t_table *p, sem_t *d, sem_t *ph);
void				after_eating(t_table *p, sem_t *d);
void				time_frame(time_t limite, time_t time);
void				wait_sem(t_table *p, sem_t *wait);
int					check_if_filled(t_table *p, int limit, sem_t *wait,
						sem_t *ph);
void				only_one_philo(t_table *p, sem_t *wait);

#endif