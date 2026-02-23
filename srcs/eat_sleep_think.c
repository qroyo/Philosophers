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

static void	ft_release_forks(t_entry *data, t_philo *philo_param);
static void	ft_count_meal(t_entry *data, t_philo *philo_param);

/*
** Print "is thinking" with alive-check.
** Lock order: is_alive -> wrt (always unlock in reverse: wrt -> is_alive).
*/
void	is_thinking(t_entry *data, t_philo *philo_param)
{
	long	time_to_print;

	philo_param->think = 1;
	pthread_mutex_lock(&data->is_alive);
	if (data->alive == 1 && data->stop == 0)
	{
		pthread_mutex_lock(&data->wrt);
		gettimeofday(&philo_param->time, NULL);
		time_to_print = ft_time(philo_param->time, data->time_start);
		printf("%ld %d is thinking\n", time_to_print, philo_param->id + 1);
		pthread_mutex_unlock(&data->wrt);
	}
	pthread_mutex_unlock(&data->is_alive);
}

/*
** Print "is sleeping" then sleep for time_to_sleep.
** Lock order: is_alive -> wrt (unlock: wrt -> is_alive).
*/
void	is_sleeping(t_entry *data, t_philo *philo_param)
{
	long	time_to_print;

	pthread_mutex_lock(&data->is_alive);
	if (data->alive == 1 && data->stop == 0)
	{
		pthread_mutex_lock(&data->wrt);
		gettimeofday(&philo_param->time, NULL);
		time_to_print = ft_time(philo_param->time, data->time_start);
		printf("%ld %d is sleeping\n", time_to_print, philo_param->id + 1);
		pthread_mutex_unlock(&data->wrt);
		pthread_mutex_unlock(&data->is_alive);
		ft_usleep(data->time_to_sleep, philo_param, data);
	}
	else
		pthread_mutex_unlock(&data->is_alive);
	philo_param->think = 0;
}

/*
** Print "has taken a fork" x2 + "is eating", update last_eat, then sleep
** for time_to_eat, release forks, and count meals.
** Lock order: is_alive -> wrt (unlock: wrt -> is_alive).
*/
void	is_eating(t_entry *data, t_philo *philo_param)
{
	long	time_to_print;

	pthread_mutex_lock(&data->is_alive);
	if (data->alive == 1 && data->stop == 0)
	{
		pthread_mutex_lock(&data->wrt);
		gettimeofday(&philo_param->time, NULL);
		time_to_print = ft_time(philo_param->time, data->time_start);
		printf("%ld %d has taken a fork\n",
			time_to_print, philo_param->id + 1);
		printf("%ld %d has taken a fork\n",
			time_to_print, philo_param->id + 1);
		printf("%ld %d is eating\n", time_to_print, philo_param->id + 1);
		pthread_mutex_unlock(&data->wrt);
		pthread_mutex_unlock(&data->is_alive);
		philo_param->last_eat = philo_param->time;
		pthread_mutex_lock(&data->last_eat_m);
		data->last_eat[philo_param->id] = philo_param->last_eat;
		pthread_mutex_unlock(&data->last_eat_m);
		ft_usleep(data->time_to_eat, philo_param, data);
		ft_release_forks(data, philo_param);
		ft_count_meal(data, philo_param);
	}
	else
	{
		pthread_mutex_unlock(&data->is_alive);
		ft_release_forks(data, philo_param);
	}
}

/*
** Release both forks atomically (one at a time, each under its own mutex).
*/
static void	ft_release_forks(t_entry *data, t_philo *philo_param)
{
	pthread_mutex_lock(&data->fork[philo_param->id]);
	data->fork_philo[philo_param->id] = 0;
	pthread_mutex_unlock(&data->fork[philo_param->id]);
	pthread_mutex_lock(&data->fork[philo_param->next_id]);
	data->fork_philo[philo_param->next_id] = 0;
	pthread_mutex_unlock(&data->fork[philo_param->next_id]);
}

/*
** Increment meal counter; if this philosopher reached nb_meal, increment
** the global philo_full counter.
*/
static void	ft_count_meal(t_entry *data, t_philo *philo_param)
{
	philo_param->nb_meal += 1;
	if (data->nb_meal != -1 && philo_param->nb_meal == data->nb_meal)
	{
		pthread_mutex_lock(&data->full_stomach);
		data->philo_full += 1;
		pthread_mutex_unlock(&data->full_stomach);
	}
}
