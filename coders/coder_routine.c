/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 10:59:36 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/13 13:54:13 by aanouer          ###   ########.fr       */
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

void	*coder_routine(void *arg)
{
	t_coder			*coder;
	struct timeval	tv;
	long long		current_time;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->sim))
	{
		take_left_dongle(coder);
		if (is_stopped(coder->sim))
			return (NULL);
		take_right_dongle(coder);
		if (is_stopped(coder->sim))
		{
			release_left_dongle(coder);
			return (NULL);
		}
		log_action(coder->sim, coder->id, "is compiling");
		gettimeofday(&tv, NULL);
		current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
		pthread_mutex_lock(&coder->sim->stop_mutex);
		coder->last_compile_time = current_time;
		coder->compile_count += 1;
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		usleep(coder->sim->time_to_compile * 1000);
		rest_of_code(coder);
	}
	return (NULL);
}
