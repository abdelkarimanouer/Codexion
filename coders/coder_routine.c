/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 10:59:36 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 14:55:12 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop_simulation;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}

static void	pass_turn(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->turn_mutex);
	sim->turn_id = (sim->turn_id % sim->number_of_coders) + 1;
	pthread_cond_broadcast(&sim->turn_cond);
	pthread_mutex_unlock(&sim->turn_mutex);
}

static void	rest_of_code(t_coder *coder)
{
	release_left_dongle(coder);
	release_right_dongle(coder);
	if (is_stopped(coder->sim))
		return ;
	log_action(coder->sim, coder->id, "is debugging");
	usleep(coder->sim->time_to_debug * 1000);
	if (is_stopped(coder->sim))
		return ;
	log_action(coder->sim, coder->id, "is refactoring");
	usleep(coder->sim->time_to_refactor * 1000);
}

static int	coder_compile_step(t_coder *coder)
{
	struct timeval	tv;
	long long		current_time;

	take_left_dongle(coder);
	if (is_stopped(coder->sim))
		return (0);
	take_right_dongle(coder);
	if (is_stopped(coder->sim))
		return (release_left_dongle(coder), 0);
	log_action(coder->sim, coder->id, "is compiling");
	gettimeofday(&tv, NULL);
	current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
	pthread_mutex_lock(&coder->sim->stop_mutex);
	coder->last_compile_time = current_time;
	coder->compile_count += 1;
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	usleep(coder->sim->time_to_compile * 1000);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->sim))
	{
		if (!wait_my_turn_or_stop(coder))
			return (NULL);
		if (!coder_compile_step(coder))
			return (NULL);
		rest_of_code(coder);
		pass_turn(coder);
	}
	return (NULL);
}
