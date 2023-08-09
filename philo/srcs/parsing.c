/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:21:08 by dsydelny          #+#    #+#             */
/*   Updated: 2023/08/07 15:42:47 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_valid_args(char **av)
{
	int	n_av;
	int	is_av;

	n_av = 1;
	while (av[n_av])
	{
		is_av = 0;
		while (av[n_av][is_av])
		{
			if (!(av[n_av][is_av] >= '0' && av[n_av][is_av] <= '9'))
			{
				printf("NOT VALID ARGUMENT: %s\n", av[n_av]);
				return (1);
			}
			is_av++;
		}
		n_av++;
	}
	return (0);
}

int	ft_atoi(char *n)
{
	int	res;
	int	i;

	res = 0;
	i = 0;
	if (n[i] == ' ' || (n[i] >= 9 && n[i] <= 13))
		i++;
	while (n[i] >= '0' && n[i] <= '9')
	{
		res = res * 10 + n[i] - '0';
		i++;
	}
	return (res);
}
