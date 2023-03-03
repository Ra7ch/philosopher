/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:42:29 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/03 14:44:36 by raitmous         ###   ########.fr       */
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

	pthread_mutex_destroy(&p->dead_lock);
	free(th);
	pthread_mutex_destroy(&p->lock1);
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

time_t	ft_time(struct timeval a, struct timeval b)
{
	time_t	sum;

	sum = (b.tv_sec * 1000 + b.tv_usec / 1000) - (a.tv_sec * 1000 + a.tv_usec
			/ 1000);
	return (sum);
}

void	philo2(void *ph, int i)
{
	t_table			*p;
	struct timeval	c;
	int				j;

	j = 0;
	p = (t_table *)ph;
	while (1)
	{
		gettimeofday(&c, NULL);
		if (check_if_filled(p, p->eat_limiter) == 0)
		{
			pthread_mutex_lock(&(p->lock1));
			return ;
		}
		j = 0;
		while (j < i)
		{
			pthread_mutex_lock(&(p->dead_lock));
			//pthread_mutex_lock(&(p->lock1));
			/* if (p->philo_dead == -1)
			{
				pthread_mutex_lock(&(p->lock1));
				return ;
			} */
			//pthread_mutex_unlock(&(p->lock1));
			pthread_mutex_unlock(&(p->dead_lock));
			if (ft_time(p->b_eat, c) > p->death_limit)
			{
				pthread_mutex_lock(&(p->lock1));
				printf("%ld %d died\n", get_time() - p->time, p->philo);
				return ;
			}
			j++;
			p = p->left;
		}
		usleep(100);
	}
	return ;
}

void	threads_create(int count, char **argv, int argc)
{
	int			i;
	t_table		*tmp;
	pthread_t	*th;
	t_table		*p;

	i = 0;
	p = build_table(count, argv, argc);
	th = malloc(count * sizeof(pthread_t));
	//pthread_mutex_lock(&(p->dead_lock));
	tmp = p;
	while (tmp->i < p->philo_count)
	{
		pthread_create(th + i, NULL, philo, p);
		pthread_detach(th[i]);
		p = p->left;
		tmp->i++;
		p->i = tmp->i;
	}
	//pthread_mutex_lock(&(p->dead_lock));
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
		printf("Unvalid number of arguments\n");
}
