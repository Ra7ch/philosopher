/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raitmous <raitmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:16:08 by raitmous          #+#    #+#             */
/*   Updated: 2023/03/04 15:18:39 by raitmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	print_error(int i, int arg)
{
	printf("ERROR:\n");
	if (i == 1)
	{
		printf("Unvalid Argument:");
		printf("Argument %d include no numeric charachter.\n", arg);
	}
	if (i == 2)
	{
		if (arg == 1)
		{
			printf("Unvalid Number: ");
			printf("Number of philosophers can't be negative.\n");
		}
		else if (arg == 5)
		{
			printf("Unvalid Number: ");
			printf("Eating count can't be negative.\n");
		}
		else
		{
			printf("Unvalid Number: ");
			printf("Time can't be negative.\n");
		}
	}
}

int	check_negative(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) < 0)
			return (print_error(2, i), 0);
		i++;
	}
	return (1);
}

int	check_no_nemeric(char **argv)
{
	int	i;
	int	j;
	int	y;

	i = 1;
	while (argv[i])
	{
		j = 0;
		y = 0;
		while (argv[i][j])
		{
			if (y == 0 && (argv[i][j] == '-' || argv[i][j] == '+'))
				j++;
			y = 1;
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (print_error(1, i), 0);
			j++;
		}
		i++;
	}
	return (1);
}

int	check_arg(char **argv)
{
	if (check_no_nemeric(argv) == 0)
		return (0);
	if (check_negative(argv) == 0)
		return (0);
	return (1);
}
