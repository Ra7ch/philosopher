/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 18:58:09 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/03 13:18:20 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	philo_sleeping(t_table *p)
{
	/* if (check_death(p, 1) == 0)
		return (0); */
	printf("at %ld philo %d is sleeping\n", get_time() - p->time, p->philo);
	eat_timer(p->sleep_time);
	/* 	if (check_death(p, 1) == 0)
		return (0); */
	printf("at %ld philo %d is thinking\n", get_time() - p->time, p->philo);
	return (1);
}

int	philo_eating(t_table *p)
{
	p->between_eating = get_time();
	gettimeofday(&p->b_eat, NULL);
	printf("at %ld philo %d is eating\n", get_time() - p->time, p->philo);
	eat_timer(p->eat_time);
	pthread_mutex_lock(&(p->left->first->lock1));
	p->eaten++;
	pthread_mutex_unlock(&(p->left->first->lock1));
	pthread_mutex_unlock(&(p->fork));
	pthread_mutex_unlock(&(p->left->fork));
	return (1);
}

int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

time_t	get_time_us(struct timeval end, struct timeval start)
{
	return ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000
			+ start.tv_usec / 1000));
}

void	eat_timer(time_t time)
{
	struct timeval	end;
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&end, NULL);
		if (get_time_us(end, start) >= time)
			break ;
		usleep(100);
	}
}

int	only_one_philo(t_table *p)
{
	if (p->philo_count == 1)
	{
		printf("at %ld philo %d is thinking\n", get_time() - p->time, 1);
		eat_timer(p->death_limit);
		printf("at %ld philo %d is dead\n", get_time() - p->time, 1);
		return (0);
	}
	return (1);
}
