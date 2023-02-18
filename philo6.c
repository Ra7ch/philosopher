/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 14:59:12 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/18 19:16:11 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo6.h"

int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	time_frame(time_t limite, time_t time, int *dead)
{
	while (get_time() - time < limite)
	{
		if (*dead == 1)
			break ;
		//	usleep(100);
	}
}

time_t	get_time_us(struct timeval end, struct timeval start)
{
	return ((end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000
			+ start.tv_usec / 1000));
}

void	eat_timer(time_t time)
{
	struct timeval	end;
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&end, NULL);
		if (get_time_us(end, start) >= time)
			break ;
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
			pthread_mutex_init(&(table->dead_lock), NULL);
			pthread_mutex_init(&(table->eat_lock), NULL);
			pthread_mutex_init(&(table->lock1), NULL);
			pthread_mutex_init(&(table->lock2), NULL);
			pthread_mutex_init(&(table->lock3), NULL);
			table->philo_dead = 0;
			table->time = get_time();
			first = table;
			table->i = 0;
		}
		table->first = first;
		table->philo = i + 1;
		table->fork_lock = 0;
		table->dead_lock = first->dead_lock;
		table->lock1 = first->lock1;
		table->lock2 = first->lock2;
		table->lock3 = first->lock3;
		table->eat_lock = first->eat_lock;
		table->philo_dead = first->philo_dead;
		table->eaten = 0;
		table->death_limit = atoi(argv[2]);
		table->eat_time = atoi(argv[3]);
		table->sleep_time = atoi(argv[4]);
		table->eat_limiter = atoi(argv[5]);
		table->philo_count = atoi(argv[1]);
		table->time = first->time;
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

int	philo_eating(t_table *p)
{
	// if (check_if_filled(p, p->eat_limiter) == 0)
	// 	return (pthread_mutex_unlock(&(p->fork)),
	// 			pthread_mutex_unlock(&(p->left->fork)),
	// 			0);
	p->between_eating = get_time();
	printf("at %ld philo %d is eating\n", get_time() - p->time, p->philo);
	eat_timer(p->eat_time);
	//time_frame(p->eat_time, get_time());
	p->eaten++;
	return (1);
}

void	*philo(void *ph)
{
	int		i;
	t_table	*p;

	//static int	*dead;
	i = 0;
	p = (t_table *)ph;
	p->between_eating = p->time;
	//	p->first->philo_dead = 0;
	//dead = malloc(4);
	//	*dead = 0;
	if (p->philo % 2)
		usleep(10000);
	while (1)
	{
		// if (p->philo % 2)
		// 	usleep(1000);
		pthread_mutex_lock(&(p->left->first->lock1));
		// printf("%ld  ,p->first->philodead == %d\n", get_time() - p->time,
		// 		p->first->philo_dead);
		if (p->first->philo_dead == 1)
			return (
				pthread_mutex_unlock(&(p->left->first->lock1)),
				pthread_mutex_unlock(&(p->dead_lock)),
				NULL);
		pthread_mutex_unlock(&(p->left->first->lock1));
		//if (p->left->fork_lock == 0 && p->fork_lock == 0)
		//{
		pthread_mutex_lock(&(p->left->fork));
		pthread_mutex_lock(&(p->fork));
		//printf("%ld  ,p->philo == %d\n", get_time() - p->time, p->philo);
		//p->left->fork_lock = 1;
		//p->fork_lock = 1;
		//pthread_mutex_unlock(&(p->left->lock1));
		i = 0;
		pthread_mutex_lock(&(p->eat_lock));
		if ((get_time() - p->between_eating) > p->death_limit)
		{
			pthread_mutex_lock(&(p->left->first->lock1));
			if (p->first->philo_dead == 1)
				return (pthread_mutex_unlock(&(p->left->first->lock1)),
						pthread_mutex_unlock(&(p->eat_lock)),
						pthread_mutex_unlock(&(p->dead_lock)),
						pthread_mutex_unlock(&(p->fork)),
						pthread_mutex_unlock(&(p->left->fork)),
						NULL);
			pthread_mutex_unlock(&(p->left->first->lock1));
			printf("at %ld philo %d is dead\n", get_time() - p->time, p->philo);
			pthread_mutex_lock(&(p->left->first->lock1));
			p->first->philo_dead = 1;
			pthread_mutex_unlock(&(p->left->first->lock1));
			return (pthread_mutex_unlock(&(p->eat_lock)),
					pthread_mutex_unlock(&(p->dead_lock)),
					pthread_mutex_unlock(&(p->fork)),
					pthread_mutex_unlock(&(p->left->fork)),
					NULL);
		}
		pthread_mutex_unlock(&(p->eat_lock));
		if (check_if_filled(p, p->eat_limiter) == 0)
		{
			pthread_mutex_lock(&(p->left->first->lock1));
			p->first->philo_dead = 1;
			pthread_mutex_unlock(&(p->left->first->lock1));
		}
		if (p->first->philo_dead == 1)
			return (
				pthread_mutex_unlock(&(p->dead_lock)),
				pthread_mutex_unlock(&(p->fork)),
				pthread_mutex_unlock(&(p->left->fork)),
				NULL);
		philo_eating(p);
		//p->fork_lock = 0;
		//p->left->fork_lock = 0;
		pthread_mutex_unlock(&(p->fork));
		pthread_mutex_unlock(&(p->left->fork));
		pthread_mutex_lock(&(p->left->first->lock1));
		if (p->first->philo_dead == 1)
			return (pthread_mutex_unlock(&(p->left->first->lock1)),
					pthread_mutex_unlock(&(p->dead_lock)),
					NULL);
		pthread_mutex_unlock(&(p->left->first->lock1));
		printf("at %ld philo %d is sleeping\n", get_time() - p->time, p->philo);
		//time_frame(p->sleep_time, get_time(), dead);
		eat_timer(p->sleep_time);
		pthread_mutex_lock(&(p->left->first->lock1));
		if (p->first->philo_dead == 1)
			return (pthread_mutex_unlock(&(p->left->first->lock1)),
					pthread_mutex_unlock(&(p->dead_lock)),
					NULL);
		pthread_mutex_unlock(&(p->left->first->lock1));
		printf("at %ld philo %d is thinking\n", get_time() - p->time, p->philo);
		//pthread_mutex_lock(&(p->left->lock1));
		//}
		//pthread_mutex_unlock(&(p->left->fork));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int i;
	t_table *p;
	t_table *tmp;
	pthread_t th[atoi(argv[1])];

	i = 0;
	if (argc == 5 || argc == 6)
	{
		p = build_table(atoi(argv[1]), argv);
		pthread_mutex_lock(&(p->dead_lock));
		tmp = p;
		while (tmp->i < p->philo_count)
		{
			pthread_create(th + i, NULL, philo, p);
			pthread_detach(th[i]);
			p = p->left;
			tmp->i++;
			p->i = tmp->i;
		}
		pthread_mutex_lock(&(p->dead_lock));
		printf("dsfsdf\n\n");
		return (0);
		// while (1)
		// {
		// 	printf("l\n");
		// 	if (philo(p) == NULL)
		// 		return (0);
		// 	//	sleep(1);
		// }
		// i = 0;
		// while (i < p->philo_count)
		// {
		// 	pthread_join(th[i], NULL);
		// 	i++;
		// }
	}
}