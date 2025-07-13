/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:14:10 by hiipek            #+#    #+#             */
/*   Updated: 2025/07/13 21:25:25 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, char **argv, int argc)
{
	if (!data)
		return (1);
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	pthread_mutex_init(&data->death_check_mutex, NULL);
	data->dead_flag = false;
	data->start_time = get_timestamp();
	pthread_mutex_init(&data->print_mutex, NULL);
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&data->forks[i]);
			}
			free(data->forks);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_init(&data->philos[i].meal_lock, NULL);
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		pthread_mutex_lock(&data->philos[i].meal_lock);
		data->philos[i].last_meal_time = get_timestamp();
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->philo_count];
		i++;
	}
	return (0);
}
