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

/*
** Precision sleep with early exit on death/stop.
** Polls the alive flag periodically to avoid holding the mutex
** in a tight loop while still exiting promptly when simulation ends.
*/
void	ft_usleep(long time, t_philo *philo_param, t_entry *data)
{
	struct timeval	time_act;
	long			new_time;
	int				running;

	if (time <= 0)
		return ;
	gettimeofday(&time_act, NULL);
	new_time = ft_time(time_act, philo_param->time);
	while (new_time < time)
	{
		pthread_mutex_lock(&data->is_alive);
		running = (data->alive == 1 && data->stop == 0);
		pthread_mutex_unlock(&data->is_alive);
		if (!running)
			return ;
		usleep(SCHEDULE_GRANULARITY_US);
		gettimeofday(&time_act, NULL);
		new_time = ft_time(time_act, philo_param->time);
	}
}
