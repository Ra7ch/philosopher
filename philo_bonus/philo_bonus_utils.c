/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 19:02:36 by raitmous          #+#    #+#             */
/*   Updated: 2023/02/26 21:21:17 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

static size_t	ftatoi(char *s)
{
	size_t	i;
	size_t	j;
	int		sign;
	size_t	tmp;

	i = 0;
	j = 0;
	tmp = 0;
	sign = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i++] == '-')
			sign = 1;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		tmp = tmp * 10 + s[i] - '0';
		if (tmp > 9223372036854775807 + (unsigned long)(sign))
			return (sign - 1);
		j = tmp;
		i++;
	}
	if (sign)
		j = j * -1;
	return (j);
}

int	ft_atoi(const char *str)
{
	char	*s;
	size_t	i;
	size_t	sign;
	int		r;

	s = (char *)str;
	i = 0;
	while (s[i] == '\t' || s[i] == ' ' || s[i] == '\r' || s[i] == '\n'
		|| s[i] == '\v' || s[i] == '\f')
		i++;
	sign = 1;
	r = ftatoi(s + i);
	return (r);
}
