/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:50:49 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/03 13:31:09 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "headers.h"

# define INT_MAX 2147483647
# define INT_MIN -2147483648

typedef struct s_request
{
	long	coder_id;
	long	ticket_number;
	long	deadline;
}		t_request;

typedef struct s_queue
{
	struct s_request	*requests;
	long				number_of_tickets;
	long				max_nums_of_tickets;
}						t_queue;

typedef struct s_dongle
{
	pthread_mutex_t		lock_dongle;
	pthread_cond_t		cond_dongle;
	int					is_available;
	long				cooldown_end;
	struct s_queue		*queue;
}						t_dongle;

typedef struct s_coder
{
	long				id;
	pthread_t			coder_thread;
	long				compile_count;
	long				last_compile_start;
	pthread_mutex_t		lock_l_c_s;
	struct s_dongle		*left_dongle;
	struct s_dongle		*right_dongle;
	struct s_simulation	*sim;
}						t_coder;

typedef struct s_simulation
{
	long			number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			number_of_compiles_required;
	long			dongle_cooldown;
	char			*scheduler;
	long			start_timestamp;
	struct s_coder	*coders;
	struct s_dongle	*dongles;
	pthread_t		monitor;
	int				stop;
	pthread_mutex_t	stop_mutex;
	long			ticket_count;
	pthread_mutex_t	ticket_count_mutex;
	pthread_mutex_t	sync_mutex;
	pthread_cond_t	sync_cond;
	int				threads_ready;
}						t_simulation;

int			parsing_args(t_simulation *sim, char **v);
void		init_simulation_with_default_values(t_simulation **sim);
void		push_request(t_queue *queue, t_request new_request,
				char *scheduler);
void		pop_request(t_queue *queue, char *scheduler);
t_request	*get_the_winner(t_queue *queue);
int			is_queue_empty(t_queue *queue);
int			init_dongles_and_coders(t_simulation **sim);
void		free_queues(t_dongle **dongles, int i);
int			init_queue_of_dongles(t_dongle **dongles,
				long number_of_coders);
int			init_mutexes_and_dongles(t_simulation *sim);
int			init_coders(t_simulation *sim);
long		get_current_time(void);
void		print_action(t_simulation *sim, int id, char *action);
int			take_dongle(t_coder *coder, t_dongle *dongle);
void		*coder_routine(void *arg);
void		my_sleep(long time_in_ms, t_simulation *sim);
void		release_dongle(t_coder *coder, t_dongle *dongle);
int			check_simulation_stop(t_simulation *sim);
void		my_sleep(long time_in_ms, t_simulation *sim);
void		*monitor_routine(void *arg);
void		*coder_routine(void *arg);
int			start_threads(t_simulation *sim);
void		join_threads(t_simulation *sim);
void		free_clean_everything(t_simulation **sim);
void		get_first_and_second_dongles(t_coder *coder, t_dongle **first,
				t_dongle **second);
int			ft_atoi(const char *str, int *failed);

#endif
