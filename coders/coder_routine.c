/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 10:59:36 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/12 15:00:59 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	rest_of_code(t_coder *coder)
{
	release_left_dongle(coder);
	release_right_dongle(coder);
	log_action(coder->sim, coder->id, "is debugging");
	usleep(coder->sim->time_to_debug * 1000);
	log_action(coder->sim, coder->id, "is refactoring");
	usleep(coder->sim->time_to_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder			*coder;
	struct timeval	tv;
	long long		current_time;
	int				should_continue;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->sim->stop_mutex);
	should_continue = (coder->sim->stop_simulation == 0);
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	while (should_continue)
	{
		take_left_dongle(coder);
		take_right_dongle(coder);
		log_action(coder->sim, coder->id, "is compiling");
		gettimeofday(&tv, NULL);
		current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
		pthread_mutex_lock(&coder->sim->stop_mutex);
		coder->last_compile_time = current_time;
		coder->compile_count += 1;
		should_continue = (coder->sim->stop_simulation == 0);
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		usleep(coder->sim->time_to_compile * 1000);
		rest_of_code(coder);
	}
	return (NULL);
}
