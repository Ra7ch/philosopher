/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:21:30 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/24 16:06:40 by raitmous         ###   ########.fr       */
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

int	check_if_filled(t_table *p, int limit, sem_t *wait, sem_t *ph)
{
	int	i;

	i = 0;
	// printf("philocount is  %d, and j is = %d\n\n", i, limit);
	// while (p->eaten >= limit && i < p->philo_count)
	// {
	// 	printf("%d >= %d && %d < %d for %d\n", p->eaten, limit, i,
	// 			p->philo_count, p->philo);
	// 	p = p->left;
	// 	printf("%d >= %d && %d < %dfor %d\n", p->eaten, limit, i,
	// 			p->philo_count, p->philo);
	// 	i++;
	// }
	// p = p->left;
	// printf("philocount is  %d, and j is = %d\n\n", i, limit);
	if (p->eaten >= limit)
		(sem_post(ph), sem_post(ph), sem_post(wait), exit(0));
	// if (i == p->philo_count)
	// 	return (0);
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

pid_t	ft_fork(t_table *p, sem_t *ph, int *dead, sem_t *d, sem_t *wait)
{
	pid_t	pid;
	time_t	time;

	//	static int	eaten;
	//int		i;
	pid = fork();
	if (pid == 0)
	{
		p->between_eating = p->time;
		while (1)
		{
			time = get_time();
			// if (*dead == 1)
			// 	(sem_post(p->first->dead), sem_post(ph), sem_post(ph), exit(1));
			sem_wait(ph);
			sem_wait(d);
			printf("at %ld philo %d has taken a fork\n", time - p->time,
					p->philo);
			sem_post(d);
			sem_wait(ph);
			sem_wait(d);
			printf("at %ld philo %d has taken a fork\n", time - p->time,
					p->philo);
			sem_post(d);
			// if (*dead == 1)
			// 	(sem_post(p->first->dead), sem_post(ph), sem_post(ph), exit(1));
			/* 	sem_wait(d);
			if (!check_if_filled(p, p->eat_limiter, wait))
			{
				printf("Im here%d\n\n\n", p->eat_limiter);
				*p->first->philo_dead = 1;
				sem_post(ph);
				sem_post(ph);
				sem_post(p->first->dead);
				exit(1);
			}
			sem_post(d); */
			//sem_wait(p->first->check_dead);
			sem_wait(d);
			if ((time - p->between_eating) > p->death_limit)
			{
				if (*dead == 1)
					(sem_post(ph), sem_post(ph), exit(1));
				printf("at %ld philo %d is dead\n", time - p->time, p->philo);
				// sem_unlink("/philo");
				// ph = sem_open("/philo", O_CREAT, 0644, 0);
				*p->first->philo_dead = 1;
				*dead = 1;
				sem_post(ph);
				sem_post(ph);
				wait_sem(p, wait);
				sem_post(p->first->dead);
				exit(1);
			}
			sem_post(d);
			// if (*dead == 1)
			// 	(sem_post(p->first->dead), sem_post(ph), sem_post(ph), exit(0));
			p->between_eating = time;
			sem_wait(d);
			printf("at %ld philo %d is eating\n", time - p->time, p->philo);
			sem_post(d);
			//usleep(p->eat_time * 1000);
			time_frame(p->eat_time, time);
			p->eaten++;
			time = get_time();
			//	printf("%deaten == %d\n", p->philo, p->eaten);
			check_if_filled(p, p->eat_limiter, wait, ph);
			// if (*dead == 1)
			// 	(sem_post(ph), sem_post(p->first->dead), sem_post(ph), exit(1));
			sem_post(ph);
			sem_post(ph);
			sem_wait(d);
			printf("at %ld philo %d is sleeping\n", time - p->time, p->philo);
			sem_post(d);
			//usleep(p->sleep_time * 1000);
			time_frame(p->sleep_time, time);
			time = get_time();
			// if (*dead == 1)
			// 	(sem_post(ph), sem_post(p->first->dead), sem_post(ph), exit(1));
			sem_wait(d);
			printf("at %ld philo %d is thinking\n", time - p->time, p->philo);
			sem_post(d);
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
	sem_t	*d;
	sem_t	*wait;
	int		*s;
	int		dead;
	int		*status;

	//int		j;
	i = 0;
	dead = 0;
	ph = malloc(sizeof(sem_t));
	if (argc == 5 || argc == 6)
	{
		p = build_table(atoi(argv[1]), argv);
		pid = malloc(sizeof(pid_t) * (p->philo_count));
		status = malloc(sizeof(int) * (p->philo_count));
		tmp = p;
		sem_unlink("/philo");
		ph = sem_open("/philo", O_CREAT, 0644, p->philo_count);
		sem_unlink("/dead");
		d = sem_open("/dead", O_CREAT, 0644, 1);
		sem_unlink("/wait");
		wait = sem_open("/wait", O_CREAT, 0644, 0);
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
			//printf("%ld<<\n", get_time() - p->time);
			pid[i] = ft_fork(p, ph, &dead, d, wait);
			p = p->left;
			i++;
		}
		// i = 0;
		// j = 0;
		// while (i < p->philo_count)
		// {
		// 	waitpid(pid[i], &status[i], 0);
		// 	printf(">%d\n\n", j);
		// 	i++;
		// }
		// i = 0;
		// j = 0;
		// while (i < p->philo_count)
		// {
		// 	if (WEXITSTATUS(status[i]) == 10)
		// 		j++;
		// 	i++;
		// }
		// if (j == p->philo_count)
		// 	sem_post(tmp->dead);
		/* while (1)
		{
			i = 0;
			printf("g\n");
			while (i < p->philo_count)
			{
				waitpid(pid[i], &status[i], 0);
				if (WEXITSTATUS(status[i]) == 1)
					break ;
				i++;
			}
			if (i != WEXITSTATUS(status[i]))
				break ;
		} */
		i = 0;
		while (i < p->philo_count)
		{
			//	printf("%d\n\n", i);
			sem_wait(wait);
			//	printf("%d\n\n", i);
			i++;
		}
		if (i == p->philo_count)
			sem_post(tmp->dead);
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