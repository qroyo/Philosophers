/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:52:41 by qroyo             #+#    #+#             */
/*   Updated: 2024/06/24 11:52:43 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	ft_routine(t_entry *data, t_philo *philo_param)
{
	pthread_mutex_lock(&data->fork[(*philo_param).next_id]);
	if (data->fork_philo[(*philo_param).next_id] == 0)
	{
		data->fork_philo[(*philo_param).next_id] = 1;
		pthread_mutex_unlock(&data->fork[(*philo_param).next_id]);
		(*philo_param) = is_eating(data, philo_param);
		if (data->nb_meal != -1)
		{
			pthread_mutex_lock(&data->full_stomach);
			if (data->philo_full == data->nb_philo)
			{
				pthread_mutex_unlock(&data->full_stomach);
				return (ft_full_stomach(data, philo_param));
			}
			else
				pthread_mutex_unlock(&data->full_stomach);
		}
		(*philo_param) = is_sleeping(data, philo_param);
	}
	else
	{
		pthread_mutex_unlock(&data->fork[(*philo_param).next_id]);
		ft_fork_unavailable(data, philo_param);
	}
	return (*philo_param);
}

void	ft_lock_and_sleep(t_entry *data, t_philo *philo_param)
{
	ft_usleep(data->time_to_eat, philo_param, data);
	pthread_mutex_lock(&data->fork[(*philo_param).id]);
	data->fork_philo[(*philo_param).id] = 0;
	pthread_mutex_unlock(&data->fork[(*philo_param).id]);
	pthread_mutex_lock(&data->fork[(*philo_param).next_id]);
	data->fork_philo[(*philo_param).next_id] = 0;
	pthread_mutex_unlock(&data->fork[(*philo_param).next_id]);
	(*philo_param).nb_meal += 1;
	if (data->nb_meal != -1)
	{
		if ((*philo_param).nb_meal == data->nb_meal)
		{
			pthread_mutex_lock(&data->full_stomach);
			data->philo_full += 1;
			pthread_mutex_unlock(&data->full_stomach);
		}
	}
}

t_philo	is_thinking(t_entry *data, t_philo *philo_param)
{
	long	time_to_print;

	philo_param->think = 1;
	pthread_mutex_lock(&data->is_alive);
	if (data->alive == 1)
	{
		pthread_mutex_lock(&data->wrt);
		gettimeofday(&(*philo_param).time, NULL);
		time_to_print = ft_time((*philo_param).time, data->time_start);
		printf("%ld %d is thinking\n", time_to_print, (*philo_param).id + 1);
		pthread_mutex_unlock(&data->is_alive);
		pthread_mutex_unlock(&data->wrt);
	}
	else
		pthread_mutex_unlock(&data->is_alive);
	return (*philo_param);
}

t_philo	is_sleeping(t_entry *data, t_philo *philo_param)
{
	long	time_to_print;

	pthread_mutex_lock(&data->is_alive);
	if (data->alive == 1)
	{
		pthread_mutex_lock(&data->wrt);
		gettimeofday(&(*philo_param).time, NULL);
		time_to_print = ft_time((*philo_param).time, data->time_start);
		printf("%ld %d is sleeping\n", time_to_print, (*philo_param).id + 1);
		pthread_mutex_unlock(&data->is_alive);
		pthread_mutex_unlock(&data->wrt);
		ft_usleep(data->time_to_sleep, philo_param, data);
		usleep(500);
	}
	else
		pthread_mutex_unlock(&data->is_alive);
	(*philo_param).think = 0;
	return (*philo_param);
}

t_philo	is_eating(t_entry *data, t_philo *philo_param)
{
	long	time_to_print;

	pthread_mutex_lock(&data->is_alive);
	if (data->alive == 1)
	{
		pthread_mutex_lock(&data->wrt);
		gettimeofday(&(*philo_param).time, NULL);
		time_to_print = ft_time((*philo_param).time, data->time_start);
		printf("%ld %d has taken a fork\n",
			time_to_print, (*philo_param).id + 1);
		printf("%ld %d has taken a fork\n",
			time_to_print, (*philo_param).id + 1);
		printf("%ld %d is eating\n", time_to_print, (*philo_param).id + 1);
		pthread_mutex_unlock(&data->wrt);
		pthread_mutex_unlock(&data->is_alive);
		(*philo_param).last_eat = (*philo_param).time;
		pthread_mutex_lock(&data->last_eat_m);
		data->last_eat[(*philo_param).id] = (*philo_param).last_eat;
		pthread_mutex_unlock(&data->last_eat_m);
		ft_lock_and_sleep(data, philo_param);
	}
	else
		pthread_mutex_unlock(&data->is_alive);
	return (*philo_param);
}
