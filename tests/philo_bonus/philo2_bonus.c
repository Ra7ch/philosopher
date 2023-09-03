/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 20:20:18 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/26 21:36:53 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	philo_eating(t_table *p, sem_t *d, sem_t *ph, sem_t *wait)
{
	time_t	time;

	time = get_time();
	sem_wait(d);
	if ((time - p->between_eating) > p->death_limit)
	{
		printf("at %ld philo %d is dead\n", time - p->time, p->philo);
		sem_post(ph);
		sem_post(ph);
		wait_sem(p, wait);
		exit(1);
	}
	p->between_eating = time;
	printf("at %ld philo %d is eating\n", time - p->time, p->philo);
	sem_post(d);
	time_frame(p->eat_time, time);
	p->eaten++;
	check_if_filled(p, p->eat_limiter, wait, ph);
	sem_post(ph);
	sem_post(ph);
}

void	hold_forks(t_table *p, sem_t *d, sem_t *ph)
{
	time_t	time;

	sem_wait(ph);
	time = get_time();
	sem_wait(d);
	printf("at %ld philo %d has taken a fork\n", time - p->time, p->philo);
	sem_post(d);
	sem_wait(ph);
	time = get_time();
	sem_wait(d);
	printf("at %ld philo %d has taken a fork\n", time - p->time, p->philo);
	sem_post(d);
}

void	after_eating(t_table *p, sem_t *d)
{
	time_t	time;

	time = get_time();
	sem_wait(d);
	printf("at %ld philo %d is sleeping\n", time - p->time, p->philo);
	sem_post(d);
	time_frame(p->sleep_time, time);
	time = get_time();
	sem_wait(d);
	printf("at %ld philo %d is thinking\n", time - p->time, p->philo);
	sem_post(d);
}

void	only_one_philo(t_table *p, sem_t *wait)
{
	if (p->philo_count == 1)
	{
		printf("at %ld philo %d is thinking\n", get_time() - p->time, 1);
		time_frame(p->death_limit, get_time());
		printf("at %ld philo %d is dead\n", get_time() - p->time, 1);
		sem_post(wait);
		exit(0);
	}
}
