/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 15:49:49 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/12 14:53:26 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophe.h"

void	*philo(void *p);

int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	time_frame(int limite, int time)
{
	while (get_time() - time != limite)
	{
		;
	}
}

void	philo_sleaping(t_philo *p)
{
	printf("at %d philo %d is sleeping\n", get_time() - p->time,
			p->place->philo);
	time_frame(p->sleep, get_time());
	//usleep(p->sleep * 1000);
}

void	philo_eating(t_philo *p)
{
	p->place->fork_lock = 0;
	p->place->left->fork_lock = 0;
	printf("at %d philo %d is eating\n", get_time() - p->time, p->place->philo);
	time_frame(p->eat, get_time());
	//usleep(p->eat * 1000);
	p->place->fork_lock = 1;
	p->place->left->fork_lock = 1;
}

void	philo_thinking(t_philo *p)
{
	printf("at %d philo %d is thinking\n", get_time() - p->time,
			p->place->philo);
}

void	*philo(void *ph)
{
	t_philo	*p;
	int		i;

	//int		j;
	//j = 0;
	p = (t_philo *)ph;
	p->between_eating = get_time();
	while (1)
	{
		pthread_mutex_lock(&p->place->fork);
		pthread_mutex_lock(&p->place->left->fork);
		philo_eating(p);
		p->eaten++;
		if (p->eaten == p->limit)
			return (0);
		p->between_eating = get_time();
		pthread_mutex_unlock(&p->place->left->fork);
		pthread_mutex_unlock(&p->place->fork);
		philo_sleaping(p);
		philo_thinking(p);
		while (p->place->fork_lock == 0 || p->place->left->fork_lock == 0)
		{
			i = get_time();
			if (i - p->between_eating >= p->death)
			{
				printf("at %d philo %d is dead\n", i - p->time,
						p->place->philo);
				p->philo_dead = 1;
				return (0);
			}
		}
	}
	return (0);
}

t_table	*build_table(int philo)
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
			first = table;
		}
		table->philo = i + 1;
		table->fork_lock = 1;
		pthread_mutex_init(&(table->fork), NULL);
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

int	main(int argc, char **argv)
{
	int			i;
	pthread_t	th[atoi(argv[1])];
	t_philo		**p;
	int			j;

	//void		*res;
	(void)argc;
	i = 0;
	p = malloc((atoi(argv[1]) + 1) * sizeof(t_philo *));
	while (i < atoi(argv[1]))
	{
		p[i] = malloc(sizeof(t_philo));
		p[i]->death = atoi(argv[2]);
		p[i]->eat = atoi(argv[3]);
		p[i]->sleep = atoi(argv[4]);
		p[i]->limit = atoi(argv[5]);
		p[i]->eaten = 0;
		p[i]->philo_dead = 0;
		p[i]->time = get_time();
		i++;
	}
	p[i] = NULL;
	(*p)->place = build_table(atoi(argv[1]));
	if (atoi(argv[1]) == 1)
	{
		printf("at %d philo %d is thinking\n", get_time() - p[0]->time, 1);
		time_frame(atoi(argv[2]), get_time());
		printf("at %d philo %d is dead\n", get_time() - p[0]->time, 1);
		return (0);
	}
	i = 1;
	while (i < atoi(argv[1]))
	{
		p[i]->place = p[i - 1]->place->right;
		i++;
	}
	i = 0;
	while (i < atoi(argv[1]))
	{
		pthread_create(th + i, NULL, philo, p[i]);
		pthread_detach(th[i]);
		i++;
	}
	i = 0;
	while (1)
	{
		j = 0;
		while (i < atoi(argv[1]))
		{
			if (p[i]->philo_dead == 1)
				break ;
			i++;
		}
		if (i != atoi(argv[1]) && p[i]->philo_dead == 1)
			break ;
	}
	j = 0;
	// while (j < atoi(argv[1]))
	// {
	// 	pthread_join(th[j], &res);
	// 	if (res == NULL)
	// 		return (0);
	// 	j++;
	// }
	//pthread_exit(0);
	printf("skdfsdf\n");
}
