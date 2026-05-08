/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 11:01:31 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/08 12:09:59 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_coder_burned_out(t_simulation *sim, t_coder *coder)
{
	int	is_dead;

	is_dead = 0;
	pthread_mutex_lock(&coder->lock_l_c_s);
	if (sim->number_of_compiles_required != -1
		&& coder->compile_count >= sim->number_of_compiles_required)
	{
		pthread_mutex_unlock(&coder->lock_l_c_s);
		return (0);
	}
	if (get_current_time() - coder->last_compile_start > sim->time_to_burnout)
		is_dead = 1;
	pthread_mutex_unlock(&coder->lock_l_c_s);
	return (is_dead);
}

static int	check_coder_burnout(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (is_coder_burned_out(sim, &sim->coders[i]))
		{
			print_action(sim, sim->coders[i].id, "burned out");
			pthread_mutex_lock(&sim->stop_sim_mutex);
			sim->stop_sim = 1;
			pthread_mutex_unlock(&sim->stop_sim_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	pthread_mutex_lock(&sim->threads_ready_mutex);
	while (sim->threads_ready == 0)
		pthread_cond_wait(&sim->threads_ready_cond, &sim->threads_ready_mutex);
	if (sim->threads_ready == -1)
	{
		pthread_mutex_unlock(&sim->threads_ready_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&sim->threads_ready_mutex);
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
