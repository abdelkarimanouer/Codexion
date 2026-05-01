/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 11:01:31 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/26 09:25:02 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_coder_burnout(t_simulation *sim)
{
	long	i;
	t_coder	*coder;

	i = 0;
	while (i < sim->number_of_coders)
	{
		coder = &sim->coders[i];
		pthread_mutex_lock(&coder->lock_l_c_s);
		if (get_current_time()
			- coder->last_compile_start > sim->time_to_burnout)
		{
			pthread_mutex_unlock(&coder->lock_l_c_s);
			print_action(sim, coder->id, "burned out");
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			return (1);
		}
		pthread_mutex_unlock(&coder->lock_l_c_s);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	pthread_mutex_lock(&sim->sync_mutex);
	while (sim->threads_ready == 0)
		pthread_cond_wait(&sim->sync_cond, &sim->sync_mutex);
	if (sim->threads_ready == -1)
	{
		pthread_mutex_unlock(&sim->sync_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&sim->sync_mutex);
	while (1)
	{
		if (check_simulation_stop(sim))
			return (NULL);
		if (check_coder_burnout(sim))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
