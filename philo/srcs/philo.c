/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:19:10 by dsydelny          #+#    #+#             */
/*   Updated: 2023/08/11 14:41:52 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	gettodaystime(void)
{
	struct timeval	current;

	if (gettimeofday(&current, NULL) == -1)
		return (-1);
	return ((current.tv_sec * 1000) + (current.tv_usec / 1000));
}

int sleeper(t_philo *philo, long duration, long start)
{
	int current;
	
	while (1)
	{
		current = gettodaystime() - start;
		if (current >= duration)
			break ;
		if (duration - current > 1000)
			usleep(100);
		else
			usleep((duration - current) / 10);
		pthread_mutex_lock(&philo->data->deathchecker);
		if (philo->data->death == 0)
		{
			pthread_mutex_unlock(&philo->data->deathchecker);
			return (1);
		}
		pthread_mutex_unlock(&philo->data->deathchecker);
	}
	return (0);
}

int	my_usleep(long int duration, t_philo *philo)
{
	long int	start;

	(void)philo;
	start = -1;
	start = gettodaystime();
	if (start == -1)
		return (1);
	else if (sleeper(philo, duration, start))
		return (1);
	return (0);
}

void	*setting_time(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->data->print);
		printf("[%ld] %i\n", gettodaystime() - philo->data->set_to_zero,
			philo->id);
		pthread_mutex_unlock(&philo->data->print);
	}
	return (0);
}

void	init(t_data *data, char **av, int ac)
{
	data->t_t_die = ft_atoi(av[2]);
	data->t_t_sleep = ft_atoi(av[3]);
	data->t_t_eat = ft_atoi(av[4]);
	data->max_eat = -1;
	if (ac == 6)
		data->max_eat = ft_atoi(av[5]);
	data->need_to_eat = data->nb_philo;
	data->death = 1;
}

int	lets_malloc(t_data *data, t_philo *philo)
{

	data->phils = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->phils)
		return (free(philo), 1);
	data->check_time = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->check_time)
		return (free(philo), 1);
	data->spoon = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->spoon)
		return (free(philo), free(data->phils), 1);
	data->set_to_zero = gettodaystime();
	data->philo = philo;
	return (0);
}

void init_philos(t_data *data, t_philo *philo){
	int	i;
	
	i = 0;
	while (i < data->nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].eaten = 0;
		philo[i].data = data;
		pthread_mutex_init(&philo[i].lunchchecker, NULL);
		philo[i].l_spoon = &(data->spoon[i]);
		if (philo[i].id == data->nb_philo)
			philo[i].r_spoon = &(data->spoon[0]);
		else
			philo[i].r_spoon = &(data->spoon[i + 1]);
		i++;
	}

}

void init_mutex(t_data *data)
{
	int i;
	
	i = 0;
	while (i < data->nb_philo)
		pthread_mutex_init(&data->spoon[i++], NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->deathchecker, NULL);
	pthread_mutex_init(&data->eatchecker, NULL);
}

int creat_thread(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->phils[i], NULL, &process_func, &philo[i]))
			return (perror("CREATION OF THREAD FAILED\n"), 1);
		i++;
	}
	i = 0;
	pthread_create(&data->watcher, NULL, &check_time_pass, data);
	while (i < data->nb_philo)
	{
		if (pthread_join(data->phils[i], NULL))
			return (perror("CREATION OF THREAD FAILED\n"), 1);
		i++;
	}
	if (pthread_join(data->watcher, NULL))
		return (perror("CREATION OF THREAD FAILED\n"), 1);
	return (0);
}

int	main(int ac, char **av)
{
	static t_data	data = {0};
	t_philo			*philo;

	if (ac < 5 || ac > 6)
		return (printf("NOT VALID AMOUNT OF ARGUMENTS\n"), 1);
	if (check_valid_args(av) || check_amount_ph(ac, av))
		exit(1);
	data.nb_philo = ft_atoi(av[1]);
	philo = calloc(sizeof(t_philo), data.nb_philo);
	if (!philo)
		return (1);
	if (lets_malloc(&data, philo))
		return (1);
	init(&data, av, ac);
	init_mutex(&data);
	init_philos(&data, philo);
	if (data.nb_philo == 1)
		return (just_one_philo(philo), 1);
	creat_thread(&data, philo);
	free(data.philo);
	free(data.spoon);
	free(data.phils);
	free(data.check_time);
}
