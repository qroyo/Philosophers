/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:44:38 by qroyo             #+#    #+#             */
/*   Updated: 2024/03/07 13:44:40 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_die(t_entry *data, int i, struct timeval *time);
void	ft_loop_watch(t_entry *data, int i, struct timeval *time);

void	*check_philo(void *arg)
{
	t_entry			*data;
	int				i;
	struct timeval	time;

	i = 0;
	data = (t_entry *) arg;
	ft_start(data);
	usleep(data->time_to_die / 2);
	ft_loop_watch(data, i, &time);
	return (NULL);
}

void	ft_loop_watch(t_entry *data, int i, struct timeval *time)
{
	pthread_mutex_lock(&data->is_alive);
	while (data->alive == 1)
	{
		pthread_mutex_unlock(&data->is_alive);
		gettimeofday(time, NULL);
		pthread_mutex_lock(&data->last_eat_m);
		if (ft_time((*time), data->last_eat[i]) > data->time_to_die)
		{
			pthread_mutex_unlock(&data->last_eat_m);
			ft_die(data, i, time);
			pthread_mutex_lock(&data->is_alive);
			break ;
		}
		else
			pthread_mutex_unlock(&data->last_eat_m);
		i++;
		if (i == data->nb_philo)
			i = 0;
		pthread_mutex_lock(&data->is_alive);
	}
	pthread_mutex_unlock(&data->is_alive);
}

void	ft_die(t_entry *data, int i, struct timeval *time)
{
	pthread_mutex_lock(&data->is_alive);
	data->alive = 0;
	pthread_mutex_lock(&data->wrt);
	printf("%ld %d died\n", \
	ft_time((*time), data->time_start), i + 1);
	pthread_mutex_unlock(&data->is_alive);
	pthread_mutex_unlock(&data->wrt);
}

void	ft_start(t_entry *data)
{
	pthread_mutex_lock(&data->start);
	pthread_mutex_unlock(&data->start);
}

long	ft_time(struct timeval t_first, struct timeval t_second)
{
	return ((t_first.tv_sec - t_second.tv_sec) * 1000 + \
	(t_first.tv_usec - t_second.tv_usec) / 1000);
}
