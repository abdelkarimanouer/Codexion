/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:50:49 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/17 16:41:09 by aanouer          ###   ########.fr       */
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

typedef struct s_request
{
	int					coder_id;
	unsigned long long	ticket_number;
	unsigned long long	deadline;
}			t_request;

typedef struct s_queue
{
	struct s_request	**requests;
	int					number_of_tickets;
	int					*max_nums_of_tickets;
}			t_queue;

typedef struct s_simulation
{
	long	number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	long	number_of_compiles_required;
	long	dongle_cooldown;
	char	*scheduler;
}			t_simulation;

int		parsing_args(t_simulation *sim, char **v);
void	init_simulation_with_default_values(t_simulation **sim);

#endif
