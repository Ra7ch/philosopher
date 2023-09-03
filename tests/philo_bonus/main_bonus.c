/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 18:40:50 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/26 21:36:35 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

t_table	*initialize_table(t_table *table, char **argv, int i, t_table *first)
{
	if (i == 0)
	{
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
	table->eaten = 0;
	table->death_limit = atoi(argv[2]);
	table->eat_time = atoi(argv[3]);
	table->sleep_time = atoi(argv[4]);
	table->philo_count = atoi(argv[1]);
	table->fork_count = atoi(argv[1]);
	table->dead = first->dead;
	table->time = first->time;
	return (first);
}

t_table	*build_table(int philo, char **argv, int argc)
{
	int		i;
	t_table	*table;
	t_table	*first;

	i = 0;
	while (i < philo)
	{
		if (i == 0)
			table = malloc(sizeof(t_table));
		first = initialize_table(table, argv, i, first);
		if (argc == 6)
			table->eat_limiter = ft_atoi(argv[5]);
		else if (argc == 5)
			table->eat_limiter = -1;
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

void	sem_initialize(t_sem *sem, t_table *p)
{
	sem_unlink("/philo");
	sem->ph = sem_open("/philo", O_CREAT, 0644, p->philo_count);
	if (sem->ph == SEM_FAILED)
		exit(1);
	sem_unlink("/dead");
	sem->d = sem_open("/dead", O_CREAT, 0644, 1);
	if (sem->d == SEM_FAILED)
		exit(1);
	sem_unlink("/wait");
	sem->wait = sem_open("/wait", O_CREAT, 0644, 0);
	if (sem->wait == SEM_FAILED)
		exit(1);
	sem->time = get_time();
}

void	philo_processes(t_table *p, t_table *tmp, t_sem *sem, pid_t *pid)
{
	int	i;

	i = 0;
	while (i < p->philo_count)
	{
		p->time = sem->time;
		pid[i] = ft_fork(p, sem->ph, sem->d, sem->wait);
		p = p->left;
		i++;
	}
	i = 0;
	while (i < p->philo_count)
	{
		sem_wait(sem->wait);
		i++;
	}
	i = 0;
	while (i < p->philo_count)
	{
		kill(pid[i], SIGTERM);
		i++;
	}
}

int	main(int argc, char **argv)
{
	int		i;
	t_table	*p;
	t_table	*tmp;
	pid_t	*pid;
	t_sem	sem;

	i = 0;
	if (argc == 5 || argc == 6)
	{
		p = build_table(atoi(argv[1]), argv, argc);
		pid = malloc(sizeof(pid_t) * (p->philo_count));
		tmp = p;
		sem_initialize(&sem, p);
		philo_processes(p, tmp, &sem, pid);
		sem_close(sem.ph);
		sem_unlink("/philo");
		sem_close(tmp->dead);
		sem_unlink("/check_dead");
	}
	else
		printf("Unvalid number of arguments\n");
}
