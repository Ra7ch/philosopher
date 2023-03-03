/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 18:27:30 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/03 14:46:19 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	check_death(t_table *p, int i)
{
	if (i == 1)
	{
		pthread_mutex_lock(&(p->left->first->lock1));
		if (p->first->philo_dead == 1)
			return (pthread_mutex_unlock(&(p->left->first->lock1)),
					pthread_mutex_unlock(&(p->dead_lock)),
					0);
		pthread_mutex_unlock(&(p->left->first->lock1));
		return (1);
	}
	if (i == 2)
	{
		pthread_mutex_lock(&(p->left->first->lock1));
		if (p->first->philo_dead == 1)
			return (pthread_mutex_unlock(&(p->left->first->lock1)),
					pthread_mutex_unlock(&(p->dead_lock)),
					pthread_mutex_unlock(&(p->fork)),
					pthread_mutex_unlock(&(p->left->fork)),
					0);
		pthread_mutex_unlock(&(p->left->first->lock1));
	}
	return (1);
}

int	if_dead(t_table *p)
{
	pthread_mutex_lock(&(p->left->first->lock1));
	if (p->first->philo_dead == 1)
		return (pthread_mutex_unlock(&(p->left->first->lock1)),
				pthread_mutex_unlock(&(p->eat_lock)),
				pthread_mutex_unlock(&(p->dead_lock)),
				pthread_mutex_unlock(&(p->fork)),
				pthread_mutex_unlock(&(p->left->fork)),
				0);
	pthread_mutex_unlock(&(p->left->first->lock1));
	printf("at %ld philo %d is dead\n", get_time() - p->time, p->philo);
	pthread_mutex_lock(&(p->left->first->lock1));
	p->first->philo_dead = 1;
	pthread_mutex_unlock(&(p->left->first->lock1));
	return (pthread_mutex_unlock(&(p->eat_lock)),
			pthread_mutex_unlock(&(p->dead_lock)),
			pthread_mutex_unlock(&(p->fork)),
			pthread_mutex_unlock(&(p->left->fork)),
			0);
}

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
		/* pthread_mutex_lock(&(p->left->first->dead_lock));
		p->philo_dead = -1;
		pthread_mutex_unlock(&(p->left->first->dead_lock)); */
		return (0);
	}
	return (1);
}

int	hold_forks(t_table *p)
{
	pthread_mutex_lock(&(p->left->fork));
	pthread_mutex_lock(&(p->eat_lock));
	/* if (check_death(p, 1) == 0)
		return (pthread_mutex_unlock(&(p->left->fork)), 0); */
	printf("at %ld philo %d has taken a fork\n", get_time() - p->time,
			p->philo);
	pthread_mutex_unlock(&(p->eat_lock));
	pthread_mutex_lock(&(p->fork));
	pthread_mutex_lock(&(p->eat_lock));
	/* if (check_death(p, 2) == 0)
		return (0); */
	printf("at %ld philo %d has taken a fork\n", get_time() - p->time,
			p->philo);
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
		/* if (check_death(p, 1) == 0)
			return (NULL); */
		if (hold_forks(p) == 0)
			return (NULL);
		/* 	if ((get_time() - p->between_eating) > p->death_limit)
			return (if_dead(p), NULL); */
		pthread_mutex_unlock(&(p->eat_lock));
		pthread_mutex_lock(&(p->eat_lock));
		//check_if_filled(p, p->eat_limiter);
		pthread_mutex_unlock(&(p->eat_lock));
		/* 	if (check_death(p, 2) == 0)
			return (NULL); */
		philo_eating(p);
		if (philo_sleeping(p) == 0)
			return (NULL);
	}
	return (0);
}
