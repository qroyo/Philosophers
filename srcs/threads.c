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

static void	ft_die(t_entry *data, int i, struct timeval *time);
static void	ft_loop_watch(t_entry *data, int i, struct timeval *time);

/*
** Watchdog thread: monitors all philosophers for death.
** Waits half of time_to_die before starting to reduce overhead.
*/
void	*check_philo(void *arg)
{
	t_entry			*data;
	int				i;
	struct timeval	time;

	i = 0;
	data = (t_entry *)arg;
	ft_start(data);
	usleep(data->time_to_die * 500);
	ft_loop_watch(data, i, &time);
	return (NULL);
}

/*
** Main watchdog loop: cycles through all philosophers,
** checking if any has exceeded time_to_die since last meal.
** Exits on death or stop (all fed).
*/
static void	ft_loop_watch(t_entry *data, int i, struct timeval *time)
{
	pthread_mutex_lock(&data->is_alive);
	while (data->alive == 1 && data->stop == 0)
	{
		pthread_mutex_unlock(&data->is_alive);
		gettimeofday(time, NULL);
		pthread_mutex_lock(&data->last_eat_m);
		if (ft_time(*time, data->last_eat[i]) > data->time_to_die)
		{
			pthread_mutex_unlock(&data->last_eat_m);
			ft_die(data, i, time);
			return ;
		}
		pthread_mutex_unlock(&data->last_eat_m);
		i++;
		if (i == data->nb_philo)
			i = 0;
		usleep(100);
		pthread_mutex_lock(&data->is_alive);
	}
	pthread_mutex_unlock(&data->is_alive);
}

/*
** Marks a philosopher as dead and prints the death message.
** Lock order: is_alive -> wrt.
** Unlock order: wrt -> is_alive (strict reverse to prevent deadlock).
*/
static void	ft_die(t_entry *data, int i, struct timeval *time)
{
	pthread_mutex_lock(&data->is_alive);
	data->alive = 0;
	pthread_mutex_lock(&data->wrt);
	printf("%ld %d died\n", ft_time(*time, data->time_start), i + 1);
	pthread_mutex_unlock(&data->wrt);
	pthread_mutex_unlock(&data->is_alive);
}

/*
** Synchronization barrier: waits until the start mutex is released
** by the main thread once all threads are created.
*/
void	ft_start(t_entry *data)
{
	pthread_mutex_lock(&data->start);
	pthread_mutex_unlock(&data->start);
}

long	ft_time(struct timeval t_first, struct timeval t_second)
{
	return ((t_first.tv_sec - t_second.tv_sec) * 1000
		+ (t_first.tv_usec - t_second.tv_usec) / 1000);
}
