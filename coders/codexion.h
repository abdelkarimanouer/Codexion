/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:50:49 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 15:18:25 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	long long			cooldown_until;
	int					is_available;
	pthread_cond_t		condition;
	struct s_coder		**waiting_queue;
	int					waiting_count;
	int					waiting_capacity;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;
	struct s_dongle		*left_dongle;
	struct s_dongle		*right_dongle;
	long long			last_compile_time;
	int					compile_count;
	long long			deadline;
	struct s_simulation	*sim;
}						t_coder;

typedef struct s_simulation
{
	long				number_of_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	long				number_of_compiles_required;
	long				dongle_cooldown;
	char				*scheduler;
	long long			start_time;
	long long			t_s_comp;
	int					stop_simulation;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		stop_mutex;
	pthread_t			monitor_thread;
	int					turn_id;
	pthread_mutex_t		turn_mutex;
	pthread_cond_t		turn_cond;
}						t_simulation;

int		parsing_args(t_simulation *sim, char **v);
int		initialize_dongles(t_simulation *sim,
			t_dongle *dongles);
void	initialize_mutexes(t_simulation *sim);
void	initialize_coders(t_simulation *sim, t_coder *coders,
			t_dongle *dongles);
int		start_threads(t_simulation *sim);
void	wait_threads(t_simulation *sim);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
void	take_left_dongle(t_coder *coder);
void	take_right_dongle(t_coder *coder);
void	release_left_dongle(t_coder *coder);
void	release_right_dongle(t_coder *coder);
void	log_action(t_simulation *sim, int coder_id, char *log);
void	add_coder_to_queue(t_dongle *dongle, t_coder *coder,
			char *scheduler);
t_coder	*get_next_coder_from_queue(t_dongle *dongle);
void	remove_coder_from_queue(t_dongle *dongle,
			t_coder *coder);
int		wait_my_turn_or_stop(t_coder *coder);
void	wake_all_waiters(t_simulation *sim);
void	wait_until_cooldown_over(t_dongle *dongle, long long cooldown_until);
void	take_dongle(t_coder *coder, t_dongle *dongle);

#endif
