/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:14:04 by hiipek            #+#    #+#             */
/*   Updated: 2025/07/15 02:42:52 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - 48;
		if ((result > INT_MAX))
			return (-1);
		i++;
	}
	return ((int)result);
}

long long	get_timestamp(void)
{
	struct timeval	time;
	long long		timestamp;

	gettimeofday(&time, NULL);
	timestamp = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (timestamp);
}

void	advanced_usleep(long long duration_time, t_data *data)
{
	long long	start_time;

	start_time = get_timestamp();
	while ((get_timestamp() - start_time < duration_time))
	{
		if (control_dead(data))
			return ;
		usleep(100);
	}
}

int	check_all_eaten(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		if (data->philos[i].meals_eaten < data->must_eat)
		{
			pthread_mutex_unlock(&data->philos[i].meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		i++;
	}
	return (1);
}

void	print_status(t_philo *philo, char *msg)
{
	long	now;

	pthread_mutex_lock(&philo->data->print_mutex);
	now = get_timestamp() - philo->data->start_time;
	if (!control_dead(philo->data))
		printf("%ld %d %s\n", now, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
