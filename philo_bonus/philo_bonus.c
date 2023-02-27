/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 18:41:10 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/26 21:01:32 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	get_time(void)
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
		(sem_post(ph), sem_post(ph), sem_post(wait), exit(0));
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
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (p->philo % 2 == 0)
			usleep(p->eat_time * 1000);
		p->between_eating = p->time;
		only_one_philo(p, wait);
		while (1)
		{
			hold_forks(p, d, ph);
			philo_eating(p, d, ph, wait);
			after_eating(p, d);
		}
	}
	return (pid);
}
