/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 20:20:18 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/02 17:52:26 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	philo_eating(t_table *p, sem_t *d, sem_t *ph, sem_t *wait)
{
	time_t	time;

	(void)wait;
	time = get_time();
	sem_wait(d);
	printf("%ld %d is eating\n", time - p->time, p->philo);
	sem_post(d);
	time_frame(p->eat_time, time);
	p->eaten++;
	check_if_filled(p, p->eat_limiter, wait, ph);
	sem_post(ph);
	sem_post(ph);
	sem_wait(d);
	p->i = 0;
	sem_post(d);
}

time_t	ft_time(struct timeval a, struct timeval b)
{
	time_t	sum;

	sum = (b.tv_sec * 1000 + b.tv_usec / 1000) - (a.tv_sec * 1000 + a.tv_usec
			/ 1000);
	return (sum);
}

void	*philo(void *ph)
{
	t_table			*p;
	struct timeval	c;

	p = (t_table *)ph;
	while (1)
	{
		gettimeofday(&c, NULL);
		if (ft_time(p->b_eat, c) > p->death_limit)
		{
			sem_wait(p->d);
			printf("%ld %d died\n", get_time() - p->time, p->philo);
			wait_sem(p, p->wait);
			exit(1);
		}
		usleep(100);
	}
	return (NULL);
}

void	hold_forks(t_table *p, sem_t *d, sem_t *ph, sem_t *wait)
{
	time_t	time;

	(void)wait;
	if (p->eaten == 9)
		printf("philo == %d here before ph\n", p->philo);
	sem_wait(ph);
	sem_wait(ph);
	sem_wait(d);
	p->i = 1;
	sem_post(d);
	time = get_time();
	if (p->eaten == 9)
		printf("philo == %d here after ph\n", p->philo);
	sem_wait(d);
	printf("%ld %d has taken a fork1\n", time - p->time, p->philo);
	printf("%ld %d has taken a fork2\n", time - p->time, p->philo);
	sem_post(d);
}

void	after_eating(t_table *p, sem_t *d)
{
	time_t	time;

	time = get_time();
	sem_wait(d);
	printf("%ld %d is sleeping\n", time - p->time, p->philo);
	sem_post(d);
	time_frame(p->sleep_time, time);
	time = get_time();
	sem_wait(d);
	printf("%ld %d is thinking\n", time - p->time, p->philo);
	sem_post(d);
}

void	only_one_philo(t_table *p, sem_t *wait)
{
	if (p->philo_count == 1)
	{
		printf("%ld %d is thinking\n", get_time() - p->time, 1);
		time_frame(p->death_limit, get_time());
		printf("%ld %d died\n", get_time() - p->time, 1);
		sem_post(wait);
		exit(0);
	}
}
