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

static void	ft_routine(t_entry *data, t_philo *philo_param);
static void	ft_loop_alive(t_entry *data, t_philo *philo_param);
static int	ft_check_stop(t_entry *data);
static int	ft_try_take_forks(t_entry *data, t_philo *philo_param);

void	*eat_sleep_think(void *arg)
{
	t_entry	*data;
	t_philo	philo_param;

	data = (t_entry *)arg;
	ft_init_philo(data, &philo_param);
	ft_loop_alive(data, &philo_param);
	return (NULL);
}

/*
** Even philosophers pick left fork first, odd pick right first.
** This asymmetry prevents circular wait (deadlock prevention).
** Returns 1 if both forks acquired, 0 otherwise.
*/
static int	ft_try_take_forks(t_entry *data, t_philo *philo_param)
{
	int	first;
	int	second;

	if (philo_param->id % 2 == 0)
	{
		first = philo_param->id;
		second = philo_param->next_id;
	}
	else
	{
		first = philo_param->next_id;
		second = philo_param->id;
	}
	pthread_mutex_lock(&data->fork[first]);
	if (data->fork_philo[first] != 0)
	{
		pthread_mutex_unlock(&data->fork[first]);
		return (0);
	}
	data->fork_philo[first] = 1;
	pthread_mutex_unlock(&data->fork[first]);
	pthread_mutex_lock(&data->fork[second]);
	if (data->fork_philo[second] != 0)
	{
		pthread_mutex_unlock(&data->fork[second]);
		pthread_mutex_lock(&data->fork[first]);
		data->fork_philo[first] = 0;
		pthread_mutex_unlock(&data->fork[first]);
		return (0);
	}
	data->fork_philo[second] = 1;
	pthread_mutex_unlock(&data->fork[second]);
	return (1);
}

static void	ft_loop_alive(t_entry *data, t_philo *philo_param)
{
	if (philo_param->id % 2 == 1)
		usleep(SCHEDULE_GRANULARITY_US);
	pthread_mutex_lock(&data->is_alive);
	while (data->alive == 1 && data->stop == 0)
	{
		pthread_mutex_unlock(&data->is_alive);
		if (ft_try_take_forks(data, philo_param))
		{
			ft_routine(data, philo_param);
			if (ft_check_stop(data))
				return ;
		}
		else
		{
			usleep(SCHEDULE_GRANULARITY_US);
		}
		pthread_mutex_lock(&data->is_alive);
	}
	pthread_mutex_unlock(&data->is_alive);
}

/*
** Checks if all philosophers have eaten enough.
** Uses a separate 'stop' flag to distinguish from death.
*/
static int	ft_check_stop(t_entry *data)
{
	if (data->nb_meal != -1)
	{
		pthread_mutex_lock(&data->full_stomach);
		if (data->nb_philo == data->philo_full)
		{
			pthread_mutex_unlock(&data->full_stomach);
			ft_full_stomach(data);
			return (1);
		}
		pthread_mutex_unlock(&data->full_stomach);
	}
	return (0);
}

static void	ft_routine(t_entry *data, t_philo *philo_param)
{
	is_eating(data, philo_param);
	if (data->nb_meal != -1)
	{
		pthread_mutex_lock(&data->full_stomach);
		if (data->philo_full == data->nb_philo)
		{
			pthread_mutex_unlock(&data->full_stomach);
			return ;
		}
		pthread_mutex_unlock(&data->full_stomach);
	}
	is_sleeping(data, philo_param);
	is_thinking(data, philo_param);
	if (data->nb_philo % 2 == 1)
		ft_usleep(data->time_to_eat / 2, philo_param, data);
}

/*
** Sets the stop flag — simulation ends because all philos are fed,
** not because one died. Keeps 'alive' semantics clean.
*/
void	ft_full_stomach(t_entry *data)
{
	pthread_mutex_lock(&data->is_alive);
	data->stop = 1;
	pthread_mutex_unlock(&data->is_alive);
}

