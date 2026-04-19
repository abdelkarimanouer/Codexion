/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:50:49 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/19 11:00:21 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "headers.h"

typedef struct s_request
{
	int					coder_id;
	unsigned long long	ticket_number;
	unsigned long long	deadline;
}			t_request;

typedef struct s_queue
{
	struct s_request	*requests;
	long				number_of_tickets;
	long				max_nums_of_tickets;
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

int			parsing_args(t_simulation *sim, char **v);
void		init_simulation_with_default_values(t_simulation **sim);
void		init_queue_with_default_values(t_queue **queue, long n_of_coders);
void		push_request(t_queue *queue, t_request new_request,
				char *scheduler);
void		pop_request(t_queue *queue, char *scheduler);
t_request	*get_the_winner(t_queue *queue);
int			is_queue_empty(t_queue *queue);

#endif
