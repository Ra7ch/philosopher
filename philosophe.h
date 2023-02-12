/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophe.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:50:05 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/12 14:48:47 by raitmous         ###   ########.fr       */
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
}					t_table;

typedef struct s_philo
{
	int				death;
	int				eat;
	int				sleep;
	int				limit;
	int				time;
	int				between_eating;
	int				eaten;
	int				philo_dead;
	t_table			*place;
}					t_philo;

#endif