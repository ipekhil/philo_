/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:14:13 by hiipek            #+#    #+#             */
/*   Updated: 2025/07/13 21:24:49 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_num(char *str)
{
	int	i;

	if (!str || !*str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static int	validate_arguments(int argc, char **argv)
{
	int	i;
	int	num;

	if (argc != 5 && argc != 6)
		return (1);
	i = 0;
	while (++i < argc)
	{
		if (is_valid_num(argv[i]))
			return (1);
		num = ft_atoi(argv[i]);
		if (num <= 0)
			return (1);
	}
	return (0);
}

int	start_simulation(t_data *data)
{
	int	i;
	pthread_t	monitor_thread;

	i = -1;
	data->start_time = get_timestamp();
	while (++i < data->philo_count)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		data->philos[i].last_meal_time = data->start_time;
		pthread_mutex_unlock(&data->philos[i].meal_lock);
	}
	i = -1;
	while (++i < data->philo_count)
	{
		pthread_create(&data->philos[i].thread_id,
			NULL, philos_life_cycle, &data->philos[i]);
	}
	if (pthread_create(&monitor_thread, NULL, monitor_philos, data) != 0)
		return (1);
	i = -1;
	while (++i < data->philo_count)
		pthread_join(data->philos[i].thread_id, NULL);
	pthread_join(monitor_thread, NULL);
	return (0);
}

void	clean_program(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->philos)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&data->philos[i].meal_lock);
			i++;
		}
		free(data->philos);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_check_mutex);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (validate_arguments(argc, argv) != 0)
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (init_data(&data, argv, argc) != 0)
		return (1);
	if (init_forks(&data) != 0)
		return (1);
	if (init_philos(&data) != 0)
		return (1);
	if (start_simulation(&data) != 0)
		return (1);
	clean_program(&data);
	return (0);
}
