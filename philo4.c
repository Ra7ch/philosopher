/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 15:19:09 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/15 14:37:43 by raitmous         ###   ########.fr       */
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
}

void	philo_eating(t_philo *p)
{
	p->place->fork_lock = 0;
	p->place->left->fork_lock = 0;
	printf("at %d philo %d is eating\n", get_time() - p->time, p->place->philo);
	time_frame(p->eat, get_time());
	p->place->eaten++;
	p->place->fork_lock = 1;
	p->place->left->fork_lock = 1;
}

void	philo_thinking(t_philo *p)
{
	printf("at %d philo %d is thinking\n", get_time() - p->time,
			p->place->philo);
}

int	check_if_filled2(t_table *place, int limit)
{
	int	i;

	i = 0;
	while (i < 30 && place->eaten >= limit)
	{
		place = place->right;
		i++;
	}
	if (i == 30)
		return (printf("eatencheck\n"), 0);
	return (1);
}

int	check_if_filled(t_table *place, int limit)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&(place->eat_lock));
	i = 0;
	while (i < 30 && place->eaten >= limit)
	{
		place = place->right;
		i++;
	}
	if (i == 30)
		return (printf("eaten1\n"), 0);
	pthread_mutex_unlock(&place->eat_lock);
	return (1);
}

void	*philo(void *ph)
{
	t_philo		*p;
	int			i;
	static int	dead;

	p = (t_philo *)ph;
	p->place->between_eating = get_time();
	while (1)
	{
		pthread_mutex_lock(&(p->place->fork));
		pthread_mutex_lock(&(p->place->left->fork));
		//pthread_mutex_lock(&(p->place->dead_lock));
		i = get_time();
		if (i - p->place->between_eating >= p->death)
		{
			printf("at %d philo %d is dead\n", i - p->time, p->place->philo);
			dead = 1;
		}
		if (dead == 1)
			return (pthread_mutex_unlock(&(p->place->left->fork)),
					pthread_mutex_unlock(&(p->place->fork)),
					NULL);
		//pthread_mutex_unlock(&(p->place->dead_lock));
		if (check_if_filled(p->place, p->limit) == 0)
			return (pthread_mutex_unlock(&(p->place->left->fork)),
					pthread_mutex_unlock(&(p->place->fork)),
					NULL);
		philo_eating(p);
		p->place->between_eating = get_time();
		pthread_mutex_unlock(&(p->place->left->fork));
		pthread_mutex_unlock(&(p->place->fork));
		philo_sleaping(p);
		philo_thinking(p);
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
			pthread_mutex_init(&(table->dead_lock), NULL);
			pthread_mutex_init(&(table->eat_lock), NULL);
			table->philo_dead = 0;
			first = table;
		}
		table->philo = i + 1;
		table->fork_lock = 1;
		table->dead_lock = first->dead_lock;
		table->eat_lock = first->eat_lock;
		table->philo_dead = first->philo_dead;
		table->eaten = 0;
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
		p[i]->count = atoi(argv[1]);
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
		//pthread_detach(th[i]);
		i++;
	}
	j = 0;
	while (j < atoi(argv[1]))
	{
		pthread_join(th[j], NULL);
		j++;
	}
	// while (1)
	// {
	// 	if (p[0]->place->philo_dead == 1)
	// 		return (printf("dead0\n"), 0);
	// 	if (check_if_filled2(p[0]->place, p[0]->limit) == 0)
	// 		return (printf("eaten%d\n", p[0]->place->eaten), 0);
	// 	j = 0;
	// }
	printf("skdfsdf\n");
}
