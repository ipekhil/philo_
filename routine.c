/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:14:21 by hiipek            #+#    #+#             */
/*   Updated: 2025/07/14 19:15:31 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		if (control_dead(philo->data))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (0);
		}
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		if (control_dead(philo->data))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	return (1);
}

void	philo_sleep_think(t_philo *philo)
{
	if (!control_dead(philo->data))
	{
		print_status(philo, "is sleeping");
		advanced_usleep(philo->data->time_to_sleep, philo->data);
	}
	if (!control_dead(philo->data))
		print_status(philo, "is thinking");
}

int	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = get_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	print_status(philo, "is eating");
	advanced_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

void	single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	if (control_dead(philo->data))
	{
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_unlock(philo->left_fork);
	return ;
}

void	*philos_life_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->philo_count == 1)
	{
		single_philo(philo);
		return (NULL);
	}
	while (!control_dead(philo->data))
	{
		if (!take_forks(philo))
			return (NULL);
		if (control_dead(philo->data))
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
		philo_eat(philo);
		philo_sleep_think(philo);
	}
	return (NULL);
}

bool	control_dead(t_data *data)
{
	bool	status;

	pthread_mutex_lock(&data->death_check_mutex);
	status = data->dead_flag;
	pthread_mutex_unlock(&data->death_check_mutex);
	return (status);
}

long long get_last_meal_time(t_philo *philo)
{
	long long last_meal_time;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_lock);
	return (last_meal_time);
}
int check_dead(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		if(get_timestamp() - get_last_meal_time(&data->philos[i]) >= data->time_to_die)
		{		
			pthread_mutex_lock(&data->death_check_mutex);
			data->dead_flag = true;
			pthread_mutex_unlock(&data->death_check_mutex);
			pthread_mutex_lock(&data->print_mutex);
			printf("%lld %d died\n", get_timestamp() - data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}
void	*monitor_philos(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!control_dead(data))
	{
		if(check_dead(data))
			return (NULL);
		if (data->must_eat > 0)
		{
			if (check_all_eaten(data))
				return (NULL);				
		}
		usleep(1000);
	}
	return (NULL);
}