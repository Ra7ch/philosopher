/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo6-1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 14:59:12 by raitmous          #+#    #+#             */
/*   Updated: 2023/09/03 03:47:49 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo6.h"

int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	time_frame(int limite, int time)
{
	while (get_time() - time < limite)
	{
		usleep(100);
	}
}

t_table	*build_table(int philo, char **argv)
{
	int		i;
	t_table	*table;
	t_table	*first;

	i = 0;
	while (i < philo)
	{
		if (i == 0)
		{
			table = malloc(sizeof(t_table));
			pthread_mutex_init(&(table->dead_lock), NULL);
			pthread_mutex_init(&(table->eat_lock), NULL);
			table->philo_dead = 0;
			table->time = get_time();
			first = table;
		}
		table->philo = i + 1;
		table->fork_lock = 1;
		table->dead_lock = first->dead_lock;
		table->eat_lock = first->eat_lock;
		table->philo_dead = first->philo_dead;
		table->eaten = 0;
		table->death_limit = atoi(argv[2]);
		table->eat_time = atoi(argv[3]);
		table->sleep_time = atoi(argv[4]);
		table->eat_limiter = atoi(argv[5]);
		table->philo_count = atoi(argv[1]);
		table->time = first->time;
		pthread_mutex_init(&(table->fork), NULL);
		i++;
		if (i == philo)
			table->right = first;
		else
			table->right = malloc(sizeof(t_table));
		table->right->left = table;
		table = table->right;
	}
	return (table);
}

int	check_if_filled(t_table *p, int limit)
{
	int	i;

	i = 0;
	while (i < p->philo_count && p->eaten >= limit)
	{
		p = p->right;
		i++;
	}
	if (i == p->philo_count)
		return (0);
	return (1);
}

int	philo_eating(t_table *p)
{
	if (check_if_filled(p, p->eat_limiter) == 0)
		return (pthread_mutex_unlock(&(p->fork)),
				pthread_mutex_unlock(&(p->left->fork)),
				0);
	p->between_eating = get_time();
	printf("at %d philo %d is eating\n", get_time() - p->time, p->philo);
	time_frame(p->eat_time, get_time());
	p->eaten++;
	return (1);
}

void	*philo(void *ph)
{
	int			i;
	t_table		*p;
	static int	*dead;

	i = 0;
	p = (t_table *)ph;
	p->between_eating = p->time;
	dead = malloc(4);
	*dead = 0;
	while (1)
	{
		if (p->philo % 2 == 0)
		{
			pthread_mutex_lock(&(p->left->fork));
			pthread_mutex_lock(&(p->fork));
			i = 0;
			if (*dead == 1)
				return (pthread_mutex_unlock(&(p->fork)),
						pthread_mutex_unlock(&(p->left->fork)),
						NULL);
			if ((get_time() - p->between_eating) > p->death_limit)
			{
				printf("at %d philo %d is dead\n", p->death_limit, (get_time()
							- p->between_eating));
				*dead = 1;
				return (pthread_mutex_unlock(&(p->fork)),
						pthread_mutex_unlock(&(p->left->fork)),
						NULL);
			}
			if (philo_eating(p) == 0)
				return (0);
			pthread_mutex_unlock(&(p->fork));
			pthread_mutex_unlock(&(p->left->fork));
			printf("at %d philo %d is sleeping\n", get_time() - p->time,
					p->philo);
			time_frame(p->sleep_time, get_time());
			printf("at %d philo %d is thinking\n", get_time() - p->time,
					p->philo);
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int i;
	t_table *p;
	pthread_t th[atoi(argv[1])];

	i = 0;
	if (argc == 5 || argc == 6)
	{
		p = build_table(atoi(argv[1]), argv);
		while (i < p->philo_count)
		{
			pthread_create(th + i, NULL, philo, p);
			p = p->left;
			i++;
		}
		i = 0;
		while (i < p->philo_count)
		{
			pthread_join(th[i], NULL);
			i++;
		}
	}
}
