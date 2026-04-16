/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:50:49 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/16 14:32:30 by aanouer          ###   ########.fr       */
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
	pthread_cond_t		condition;
	long long			cooldown_until;
	int					is_available;
}						t_dongle;

typedef struct s_coder
{
	int						id;
	pthread_t				thread;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
	long long				last_compile_start;
	int						compile_count;
	struct s_simulation		*sim;
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
	int					stop;
	int					burned_out_coder_id;
	pthread_t			monitor_thread;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		print_mutex;
	t_coder				*coders;
	t_dongle			*dongles;
}						t_simulation;

int		parsing_args(t_simulation *sim, char **v);

#endif
