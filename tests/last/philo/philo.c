/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 18:27:30 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/04 13:21:11 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	check_if_filled(t_table *p, int limit)
{
	int	i;

	p->j = 0;
	if (limit == -1)
		return (1);
	i = 0;
	pthread_mutex_lock(&(p->left->first->lock1));
	while (i < p->philo_count && p->eaten >= limit)
	{
		p = p->right;
		i++;
	}
	pthread_mutex_unlock(&(p->left->first->lock1));
	if (i == p->philo_count)
	{
		return (0);
	}
	return (1);
}

int	hold_forks(t_table *p)
{
	pthread_mutex_lock(&(p->left->fork));
	pthread_mutex_lock(&(p->eat_lock));
	pthread_mutex_lock(&(p->first->lock1));
	printf("%ld %d has taken a fork\n", get_time() - p->time, p->philo);
	pthread_mutex_unlock(&(p->first->lock1));
	pthread_mutex_unlock(&(p->eat_lock));
	pthread_mutex_lock(&(p->fork));
	pthread_mutex_lock(&(p->eat_lock));
	pthread_mutex_lock(&(p->first->lock1));
	printf("%ld %d has taken a fork\n", get_time() - p->time, p->philo);
	pthread_mutex_unlock(&(p->first->lock1));
	pthread_mutex_unlock(&(p->eat_lock));
	pthread_mutex_lock(&(p->eat_lock));
	return (1);
}

void	*philo(void *ph)
{
	t_table	*p;

	p = (t_table *)ph;
	p->between_eating = p->time;
	gettimeofday(&p->b_eat, NULL);
	if (p->philo % 2 == 0)
		usleep(10000);
	if (only_one_philo(p) == 0)
		return (pthread_mutex_unlock(&(p->dead_lock)), NULL);
	while (1)
	{
		if (hold_forks(p) == 0)
			return (NULL);
		pthread_mutex_unlock(&(p->eat_lock));
		pthread_mutex_lock(&(p->eat_lock));
		pthread_mutex_unlock(&(p->eat_lock));
		philo_eating(p);
		if (philo_sleeping(p) == 0)
			return (NULL);
	}
	return (0);
}

time_t	ft_time(struct timeval a, struct timeval b)
{
	time_t	sum;

	sum = (b.tv_sec * 1000 + b.tv_usec / 1000) - (a.tv_sec * 1000 + a.tv_usec
			/ 1000);
	return (sum);
}

void	philo2(t_table *p, int i)
{
	struct timeval	c;
	int				j;

	while (1)
	{
		gettimeofday(&c, NULL);
		if (check_if_filled(p, p->eat_limiter) == 0)
		{
			pthread_mutex_lock(&(p->first->lock1));
			return ;
		}
		j = 0;
		while (j < i)
		{
			if (ft_time(p->b_eat, c) > p->death_limit)
			{
				pthread_mutex_lock(&(p->first->lock1));
				printf("%ld %d died\n", get_time() - p->time, p->philo);
				return ;
			}
			j++;
			p = p->left;
		}
		usleep(100);
	}
}
