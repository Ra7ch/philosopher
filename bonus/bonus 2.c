/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:21:30 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/22 20:41:16 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo6.h"

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
			table->philo_dead = malloc(4);
			*table->philo_dead = 0;
			table->time = get_time();
			first = table;
			table->i = 0;
			sem_unlink("/check_dead");
			table->dead = sem_open("/check_dead", O_CREAT, 0644, 0);
			if (table->dead == SEM_FAILED)
				exit(1);
		}
		table->first = first;
		table->philo = i + 1;
		//table->philo_dead = first->philo_dead;
		table->eaten = 0;
		table->death_limit = atoi(argv[2]);
		table->eat_time = atoi(argv[3]);
		table->sleep_time = atoi(argv[4]);
		table->eat_limiter = atoi(argv[5]);
		table->philo_count = atoi(argv[1]);
		table->fork_count = atoi(argv[1]);
		table->time = first->time;
		table->dead = first->dead;
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
	//int	i;
	p->j = 0;
	while (p->j < p->philo_count && p->eaten >= limit)
	{
		p = p->right;
		p->j++;
	}
	if (p->j == p->philo_count)
		return (0);
	return (1);
}

pid_t	ft_fork(t_table *p, sem_t *ph)
{
	pid_t	pid;

	//int		i;
	pid = fork();
	if (pid == 0)
	{
		p->between_eating = p->time;
		while (1)
		{
			if (*p->first->philo_dead == 1)
				(sem_post(p->first->dead), sem_post(ph), sem_post(ph), exit(1));
			sem_wait(ph);
			sem_wait(ph);
			if (*p->first->philo_dead == 1)
				(sem_post(p->first->dead), sem_post(ph), sem_post(ph), exit(1));
			if (!check_if_filled(p, p->eat_limiter))
			{
				*p->first->philo_dead = 1;
				sem_post(ph);
				sem_post(ph);
				sem_post(p->first->dead);
				exit(1);
			}
			if ((get_time() - p->between_eating) > p->death_limit)
			{
				if (*p->first->philo_dead == 1)
					(sem_post(ph), sem_post(ph), exit(1));
				printf("at %ld philo %d is dead\n", get_time() - p->time,
						p->philo);
				*p->first->philo_dead = 1;
				sem_post(ph);
				sem_post(ph);
				sem_post(p->first->dead);
				exit(1);
			}
			if (*p->first->philo_dead == 1)
				(sem_post(p->first->dead), sem_post(ph), sem_post(ph), exit(0));
			p->between_eating = get_time();
			printf("at %ld philo %d is eating\n", get_time() - p->time,
					p->philo);
			//usleep(p->eat_time * 1000);
			time_frame(p->eat_time, get_time());
			p->eaten++;
			if (*p->first->philo_dead == 1)
				(sem_post(ph), sem_post(p->first->dead), sem_post(ph), exit(1));
			sem_post(ph);
			sem_post(ph);
			printf("at %ld philo %d is sleeping\n", get_time() - p->time,
					p->philo);
			//usleep(p->sleep_time * 1000);
			time_frame(p->sleep_time, get_time());
			if (*p->first->philo_dead == 1)
				(sem_post(ph), sem_post(p->first->dead), sem_post(ph), exit(1));
			printf("at %ld philo %d is thinking\n", get_time() - p->time,
					p->philo);
		}
	}
	return (pid);
}

int	main(int argc, char **argv)
{
	int		i;
	t_table	*p;
	t_table	*tmp;
	pid_t	*pid;
	sem_t	*ph;
	int		*s;

	//int		status;
	i = 0;
	ph = malloc(sizeof(sem_t));
	if (argc == 5 || argc == 6)
	{
		p = build_table(atoi(argv[1]), argv);
		pid = malloc(sizeof(pid_t) * (p->philo_count));
		tmp = p;
		sem_unlink("/philo");
		ph = sem_open("/philo", O_CREAT, 0644, p->philo_count);
		s = malloc(4);
		*s = 2;
		if (ph == (sem_t *)s)
			exit(0);
		if (ph == SEM_FAILED)
			exit(2);
		// if (p->philo % 2)
		// 	usleep(10000);
		while (i < p->philo_count)
		{
			p->time = get_time();
			printf("%ld<<\n", get_time() - p->time);
			pid[i] = ft_fork(p, ph);
			p = p->left;
			i++;
		}
		sem_wait(tmp->dead);
		//i--;
		//while (i >= 0)
		//{
		//	waitpid(pid[i], &status, 0);
		//if (WEXITSTATUS(status) != 0)
		//	{
		//		printf("gg\n\n\n");
		i = 0;
		while (i < p->philo_count)
		{
			kill(pid[i], SIGTERM);
			i++;
		}
		//		exit(0);
		//	}
		//	i--;
		//}
		// while (1)
		// {
		// 	if (tmp->philo_dead == 1)
		// 		exit(0);
		// }
		sem_close(ph);
		sem_unlink("/philo");
		sem_close(tmp->dead);
		sem_unlink("/check_dead");
		// i--;
		// while (i >= 0)
		// {
		// 	waitpid(pid[i], NULL, 0);
		// 	i--;
		// }
	}
}

//use semaphore to shre dead value between forks?...