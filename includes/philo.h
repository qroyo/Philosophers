/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qroyo <qroyo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:42:48 by qroyo             #+#    #+#             */
/*   Updated: 2024/02/20 11:42:51 by qroyo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdint.h>
# include <stddef.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_entry
{
	int				*next_id;
	int				*fork_philo;
	int				*id_philo;
	long			nb_philo;
	long			time_to_die;
	long			time_to_sleep;
	long			time_to_eat;
	long			nb_meal;
	long			philo_full;
	pthread_t		*philo_tab;
	pthread_t		check_philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	mutex_id;
	pthread_mutex_t	wrt;
	pthread_mutex_t	start;
	pthread_mutex_t	is_alive;
	pthread_mutex_t	last_eat_m;
	pthread_mutex_t	id_data;
	pthread_mutex_t	full_stomach;
	int				alive;
	struct timeval	*last_eat;
	struct timeval	time_start;
}	t_entry;

typedef struct s_philo
{
	int				id;
	int				next_id;
	int				think;
	struct timeval	time;
	struct timeval	last_eat;
	long			nb_meal;
}	t_philo;

t_entry	*ft_parse(int argc, char **argv);
void	*eat_sleep_think(void *arg);
int		ft_init(t_entry *data);
void	*check_philo(void *arg);
long	ft_time(struct timeval t_first, struct timeval t_second);
void	ft_start(t_entry *data);
void	ft_usleep(long time, t_philo *philo_param, t_entry *data);
t_philo	is_eating(t_entry *data, t_philo *philo_param);
t_philo	is_sleeping(t_entry *data, t_philo *philo_param);
t_philo	is_thinking(t_entry *data, t_philo *philo_param);
t_philo	ft_init_philo(t_entry *data, t_philo *philo_param);
t_philo	ft_full_stomach(t_entry *data, t_philo *philo_param);
void	ft_fork_unavailable(t_entry *data, t_philo *philo_param);

#endif
