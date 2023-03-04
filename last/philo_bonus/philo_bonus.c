/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 18:41:10 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/04 13:28:52 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

sem_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	time_frame(time_t limite, time_t time)
{
	while (get_time() - time < limite)
	{
		usleep(100);
	}
}

int	check_if_filled(t_table *p, int limit, sem_t *wait, sem_t *ph)
{
	int	i;

	i = 0;
	if (limit == -1)
		return (1);
	if (p->eaten >= limit)
	{
		sem_post(ph);
		sem_post(ph);
		sem_post(wait);
		exit(1);
	}
	return (1);
}

void	wait_sem(t_table *p, sem_t *wait)
{
	int	i;

	i = 0;
	while (i < p->philo_count)
	{
		sem_post(wait);
		i++;
	}
}

pid_t	ft_fork(t_table *p, sem_t *ph, sem_t *d, sem_t *wait)
{
	pid_t		pid;
	pthread_t	th;

	(void)th;
	pid = fork();
	if (pid == 0)
	{
		only_one_philo(p, wait);
		p->d = d;
		p->wait = wait;
		p->ph = ph;
		gettimeofday(&p->b_eat, NULL);
		pthread_create(&th, NULL, philo, p);
		pthread_detach(th);
		while (1)
		{
			hold_forks(p, d, ph, wait);
			sem_wait(d);
			gettimeofday(&p->b_eat, NULL);
			sem_post(d);
			philo_eating(p, d, ph, wait);
			after_eating(p, d);
		}
	}
	return (pid);
}

/* pid_t	ft_fork(t_table *p, sem_t *ph, sem_t *d, sem_t *wait)
{
	pid_t		pid;
	pthread_t	th;

	pid = fork();
	if (pid == 0)
	{
		if (p->philo % 2 == 0)
			usleep(10000);
		sem_wait(d);
		p->between_eating = malloc(8);
		*p->between_eating = p->time;
		sem_post(d);
		only_one_philo(p, wait);
		sem_wait(d);
		p->d = d;
		p->wait = wait;
		pthread_create(&th, NULL, philo, p);
		pthread_detach(th);
		sem_post(d);
		while (1)
		{
			hold_forks(p, d, ph, wait);
			philo_eating(p, d, ph, wait);
			after_eating(p, d);
		}
	}
	return (pid);
} */
