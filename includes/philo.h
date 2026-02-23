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

# define SCHEDULE_GRANULARITY_US	500
# define MAX_PHILOSOPHERS			1000

typedef struct s_entry
{
	int				*fork_philo;
	int				*id_philo;
	long			nb_philo;
	long			time_to_die;
	long			time_to_sleep;
	long			time_to_eat;
	long			nb_meal;
	long			philo_full;
	int				alive;
	int				stop;
	pthread_t		*philo_tab;
	pthread_t		check_philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	wrt;
	pthread_mutex_t	start;
	pthread_mutex_t	is_alive;
	pthread_mutex_t	last_eat_m;
	pthread_mutex_t	id_data;
	pthread_mutex_t	full_stomach;
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

/* parsing.c */
t_entry	*ft_parse(int argc, char **argv);

/* routine.c */
void	*eat_sleep_think(void *arg);
void	ft_full_stomach(t_entry *data);

/* eat_sleep_think.c */
void	is_eating(t_entry *data, t_philo *philo_param);
void	is_sleeping(t_entry *data, t_philo *philo_param);
void	is_thinking(t_entry *data, t_philo *philo_param);

/* threads.c */
void	*check_philo(void *arg);
void	ft_start(t_entry *data);
long	ft_time(struct timeval t_first, struct timeval t_second);

/* init.c */
int		ft_init(t_entry *data);
void	ft_init_philo(t_entry *data, t_philo *philo_param);

/* init_destroy_fork.c */
int		ft_init_fork(t_entry *data);
void	ft_destroy_fork(t_entry *data, int check_fork);

/* init_mutex.c */
int		ft_init_mutex(t_entry *data);
void	ft_destroy_init(int check_init, t_entry *data);

/* init_free_malloc.c */
int		ft_init_malloc(t_entry *data);
void	ft_free(int check_malloc, t_entry *data);

/* ft_usleep.c */
void	ft_usleep(long time, t_philo *philo_param, t_entry *data);

/* main.c */
void	ft_clean_philo(t_entry *data);

#endif
