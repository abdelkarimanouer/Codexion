/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 08:42:07 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/08 18:17:56 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	handle_burnout_broadcast(t_simulation *sim)
{
	long	i;

	sim->stop_sim = 1;
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_cond_broadcast(&sim->dongles[i].cond_dongle);
		i++;
	}
}

void	print_action(t_simulation *sim, int id, char *action)
{
	int		is_stopped;
	long	timestamp;

	pthread_mutex_lock(&sim->stop_sim_mutex);
	is_stopped = sim->stop_sim;
	if (is_stopped)
	{
		pthread_mutex_unlock(&sim->stop_sim_mutex);
		return ;
	}
	if (action[0] == 'b')
		handle_burnout_broadcast(sim);
	timestamp = get_current_time() - sim->start_timestamp;
	printf("%ld %d %s\n", timestamp, id, action);
	pthread_mutex_unlock(&sim->stop_sim_mutex);
}
