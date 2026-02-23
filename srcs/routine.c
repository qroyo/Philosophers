/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 10:32:03 by qroyo             #+#    #+#             */
/*   Updated: 2024/03/11 10:32:05 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	ft_routine(t_entry *data, t_philo *philo_param);
void	ft_lock_and_sleep(t_entry *data, t_philo *philo_param);
void	ft_loop_alive(t_entry *data, t_philo *philo_param);
void	ft_fork_unavailable(t_entry *data, t_philo *philo_param);
t_philo	ft_full_stomach(t_entry *data, t_philo *philo_param);

void	*eat_sleep_think(void *arg)
{
	t_entry	*data ;
	t_philo	philo_param;

	data = (t_entry *) arg;
	philo_param = ft_init_philo(data, &philo_param);
	ft_loop_alive(data, &philo_param);
	return (NULL);
}

int	ft_check_full(t_entry *data)
{
	if (data->nb_meal != -1)
	{
		pthread_mutex_lock(&data->full_stomach);
		if (data->nb_philo == data->philo_full)
		{
			pthread_mutex_unlock(&data->full_stomach);
			pthread_mutex_lock(&data->is_alive);
			return (1);
		}
		pthread_mutex_unlock(&data->full_stomach);
	}
	return (0);
}

void	ft_loop_alive(t_entry *data, t_philo *philo_param)
{
	if (((*philo_param).id + 1) % 2 == 1)
		usleep(500);
	pthread_mutex_lock(&data->is_alive);
	while (data->alive == 1)
	{
		pthread_mutex_unlock(&data->is_alive);
		pthread_mutex_lock(&data->fork[(*philo_param).id]);
		if (data->fork_philo[(*philo_param).id] == 0)
		{
			data->fork_philo[(*philo_param).id] = 1;
			pthread_mutex_unlock(&data->fork[(*philo_param).id]);
			(*philo_param) = ft_routine(data, philo_param);
			if (ft_check_full(data) == 1)
				break ;
		}
		else
		{
			pthread_mutex_unlock(&data->fork[(*philo_param).id]);
			if ((*philo_param).think == 0)
				(*philo_param) = is_thinking(data, philo_param);
		}
		pthread_mutex_lock(&data->is_alive);
	}
	pthread_mutex_unlock(&data->is_alive);
}

t_philo	ft_full_stomach(t_entry *data, t_philo *philo_param)
{
	pthread_mutex_lock(&data->is_alive);
	data->alive = 0;
	pthread_mutex_unlock(&data->is_alive);
	return (*philo_param);
}

void	ft_fork_unavailable(t_entry *data, t_philo *philo_param)
{
	pthread_mutex_lock(&data->fork[(*philo_param).id]);
	data->fork_philo[(*philo_param).id] = 0;
	pthread_mutex_unlock(&data->fork[(*philo_param).id]);
	if ((*philo_param).think == 0)
		(*philo_param) = is_thinking(data, philo_param);
}
