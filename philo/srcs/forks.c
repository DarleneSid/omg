/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 15:21:21 by dsydelny          #+#    #+#             */
/*   Updated: 2023/08/09 23:35:19 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*print_msg(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->deathchecker);
	if (!philo->data->death)
	{
		pthread_mutex_unlock(&philo->data->deathchecker);
		return ("DIED");
	}
	pthread_mutex_unlock(&philo->data->deathchecker);
	pthread_mutex_lock(&philo->data->print);
	printf("%ld	№%d %s\n", gettodaystime() - philo->data->set_to_zero,
		philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
	return (NULL);
}

void	*die_right_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->r_spoon);
		pthread_mutex_unlock(philo->l_spoon);
		return ("DIED");
	}
	if (print_msg(philo, "is eating"))
	{
		pthread_mutex_unlock(philo->r_spoon);
		pthread_mutex_unlock(philo->l_spoon);
		return ("DIED");
	}
	return (NULL);
}

void	*right_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->r_spoon);
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->r_spoon);
		return ("DIED");
	}
	pthread_mutex_lock(philo->l_spoon);
	if (die_right_fork(philo))
		return ("DIED");
	philo->eaten++;
	if (philo->data->max_eat != -1 && philo->eaten == philo->data->max_eat)
	{
		pthread_mutex_lock(&philo->data->eatchecker);
		philo->data->need_to_eat--;
		pthread_mutex_unlock(&philo->data->eatchecker);
	}
	pthread_mutex_lock(&philo->lunchchecker);
	philo->last_lunch = gettodaystime() - philo->data->set_to_zero;
	pthread_mutex_unlock(&philo->lunchchecker);
	my_usleep(philo->data->t_t_eat, philo);
	pthread_mutex_unlock(philo->r_spoon);
	pthread_mutex_unlock(philo->l_spoon);
	return (NULL);
}

void	*die_left_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->l_spoon);
		pthread_mutex_unlock(philo->r_spoon);
		return ("DIED");
	}
	if (print_msg(philo, "is eating"))
	{
		pthread_mutex_unlock(philo->l_spoon);
		pthread_mutex_unlock(philo->r_spoon);
		return ("DIED");
	}
	return (NULL);
}

void	*left_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->l_spoon);
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->l_spoon);
		return ("DIED");
	}
	pthread_mutex_lock(philo->r_spoon);
	if (die_left_fork(philo))
		return ("DIED");
	philo->eaten++;
	if (philo->data->max_eat != -1 && philo->eaten == philo->data->max_eat)
	{
		pthread_mutex_lock(&philo->data->eatchecker);
		philo->data->need_to_eat--;
		pthread_mutex_unlock(&philo->data->eatchecker);
	}
	pthread_mutex_lock(&philo->lunchchecker);
	philo->last_lunch = gettodaystime() - philo->data->set_to_zero;
	pthread_mutex_unlock(&philo->lunchchecker);
	my_usleep(philo->data->t_t_eat, philo);
	pthread_mutex_unlock(philo->l_spoon);
	pthread_mutex_unlock(philo->r_spoon);
	return (NULL);
}
