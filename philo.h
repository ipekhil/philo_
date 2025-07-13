/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiipek <hiipek@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:14:19 by hiipek            #+#    #+#             */
/*   Updated: 2025/07/13 21:25:34 by hiipek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>

# define INT_MAX	2147483647
# define INT_MIN	-2147483648

typedef struct s_data t_data;

typedef struct s_philo
{
    int				id;
    int				meals_eaten;
    long long		last_meal_time;
    pthread_mutex_t meal_lock;
    pthread_t		thread_id;
    pthread_mutex_t	*left_fork;
    pthread_mutex_t	*right_fork;
    t_data			*data;
}   t_philo;

typedef struct s_data
{
    int				philo_count;
    int				time_to_die;
    int				time_to_eat;
    int				time_to_sleep;
    int				must_eat;

    bool			dead_flag;
    long long		start_time;

    pthread_mutex_t	*forks;
    pthread_mutex_t	print_mutex;
    pthread_mutex_t	death_check_mutex;

    t_philo			*philos;
}	t_data;

long long	get_timestamp(void);
void		advanced_usleep(long long duration_time, t_data *data);
void		print_status(t_philo *philo, char *msg);
int         ft_atoi(char *str);
void	    *philos_life_cycle(void *arg);
void        *monitor_philos(void *arg);
void        print_status(t_philo *philo, char *msg);


int	init_data(t_data *data,char **argv, int argc);
int	init_philos(t_data *data);
int	init_forks(t_data *data);


bool	control_dead(t_data *data);
int	check_all_eaten(t_data *data);

#endif