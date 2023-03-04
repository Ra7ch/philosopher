/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:42:29 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/04 15:18:00 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

t_table	*initialize_table(t_table *table, char **argv, int i, t_table *first)
{
	if (i == 0)
	{
		pthread_mutex_init(&(table->dead_lock), NULL);
		pthread_mutex_init(&(table->eat_lock), NULL);
		pthread_mutex_init(&(table->lock1), NULL);
		table->philo_dead = 0;
		table->time = get_time();
		table->i = 0;
		first = table;
	}
	table->first = first;
	table->philo = i + 1;
	table->fork_lock = 0;
	table->dead_lock = first->dead_lock;
	table->lock1 = first->lock1;
	table->eat_lock = first->eat_lock;
	table->philo_dead = first->philo_dead;
	table->eaten = 0;
	table->death_limit = ft_atoi(argv[2]);
	table->eat_time = ft_atoi(argv[3]);
	table->sleep_time = ft_atoi(argv[4]);
	table->philo_count = ft_atoi(argv[1]);
	table->time = first->time;
	pthread_mutex_init(&(table->fork), NULL);
	return (first);
}

void	ft_free(pthread_t *th, t_table *p, int count, t_table *tmp)
{
	int	i;

	free(th);
	i = 0;
	while (i < count)
	{
		tmp = p->left;
		free(p);
		p = tmp;
		i++;
	}
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

void	threads_create(int count, char **argv, int argc)
{
	int			i;
	t_table		*tmp;
	pthread_t	*th;
	t_table		*p;

	i = 0;
	if (check_arg(argv) == 0)
		return ;
	p = build_table(count, argv, argc);
	th = malloc(count * sizeof(pthread_t));
	tmp = p;
	while (tmp->i < p->philo_count)
	{
		pthread_create(th + i, NULL, philo, p);
		pthread_detach(th[i]);
		p = p->left;
		tmp->i++;
		p->i = tmp->i;
	}
	philo2(p, p->philo_count);
	ft_free(th, p, count, tmp);
}

int	main(int argc, char **argv)
{
	int	count;

	if (argc == 5 || argc == 6)
	{
		count = ft_atoi(argv[1]);
		threads_create(count, argv, argc);
		return (0);
	}
	else
		printf("ERROR:\nUnvalid number of arguments.\n");
}
