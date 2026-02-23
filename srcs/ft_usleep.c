/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 11:34:25 by qroyo             #+#    #+#             */
/*   Updated: 2024/06/24 11:34:27 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long time, t_philo *philo_param, t_entry *data)
{
	struct timeval	time_act;
	long			new_time;

	if (time != 0)
	{
		gettimeofday(&time_act, NULL);
		new_time = ft_time(time_act, (*philo_param).time);
		pthread_mutex_lock(&data->is_alive);
		while (new_time < time && data->alive == 1)
		{
			pthread_mutex_unlock(&data->is_alive);
			usleep(500);
			gettimeofday(&time_act, NULL);
			new_time = ft_time(time_act, (*philo_param).time);
			pthread_mutex_lock(&data->is_alive);
		}
		pthread_mutex_unlock(&data->is_alive);
	}
}
