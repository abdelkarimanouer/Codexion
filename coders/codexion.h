/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:50:49 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/16 13:22:04 by aanouer          ###   ########.fr       */
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
}						t_simulation;

int		parsing_args(t_simulation *sim, char **v);

#endif
