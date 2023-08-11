/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:49:29 by dsydelny          #+#    #+#             */
/*   Updated: 2023/08/11 14:44:42 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*food_check(t_data *data)
{
	pthread_mutex_lock(&data->eatchecker);
	if (!data->need_to_eat)
	{
		pthread_mutex_unlock(&data->eatchecker);
		pthread_mutex_lock(&data->deathchecker);
		data->death = 0;
		pthread_mutex_unlock(&data->deathchecker);
		return (NULL);
	}
	pthread_mutex_unlock(&data->eatchecker);
	return (NULL);
}

void	*case_death(t_data *data)
{
	pthread_mutex_lock(&data->deathchecker);
	data->death = 0;
	pthread_mutex_unlock(&data->deathchecker);
	return (NULL);
}

void	*check_time_pass(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->philo[i].lunchchecker);
			if (gettodaystime() - data->set_to_zero
				- data->philo[i].last_lunch >= data->t_t_die)
			{
				pthread_mutex_unlock(&data->philo[i].lunchchecker);
				print_msg(data->philo, "died");
				case_death(data);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philo[i].lunchchecker);
			food_check(data);
			i++;
		}
	}
	return (NULL);
}

void	*death_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->deathchecker);
	if (!philo->data->death)
	{
		pthread_mutex_unlock(&philo->data->deathchecker);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->data->deathchecker);
	if (philo->id % 2 == 0)
	{
		if (right_fork(philo))
			return (NULL);
	}
	else
	{
		if (left_fork(philo))
			return (NULL);
	}
	return (NULL);
}

void	*process_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(10);
	while (1)
	{
		if ((philo->data->nb_philo % 2 != 0) && philo->eaten > 0)
		{
			usleep(philo->data->t_t_die * 0.25);
		}
		death_check(philo);
		if (print_msg(philo, "is sleeping"))
			return (NULL);
		my_usleep(philo->data->t_t_sleep, philo);
		if (print_msg(philo, "is thinking"))
			return (NULL);
	}
	return (NULL);
}
