/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 11:02:55 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/08 12:07:40 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	do_compile(t_coder *coder, t_dongle *first, t_dongle *second)
{
	if (!take_dongle(coder, first))
		return (0);
	if (!take_dongle(coder, second))
	{
		release_dongle(coder, first);
		return (0);
	}
	print_action(coder->sim, coder->id, "is compiling");
	pthread_mutex_lock(&coder->lock_l_c_s);
	coder->last_compile_start = get_current_time();
	pthread_mutex_unlock(&coder->lock_l_c_s);
	my_sleep(coder->sim->time_to_compile, coder->sim);
	pthread_mutex_lock(&coder->lock_l_c_s);
	coder->compile_count++;
	coder->last_compile_start = get_current_time();
	pthread_mutex_unlock(&coder->lock_l_c_s);
	release_dongle(coder, first);
	release_dongle(coder, second);
	return (1);
}

static void	do_debug_refactor(t_coder *coder)
{
	print_action(coder->sim, coder->id, "is debugging");
	my_sleep(coder->sim->time_to_debug, coder->sim);
	if (check_simulation_stop(coder->sim))
		return ;
	print_action(coder->sim, coder->id, "is refactoring");
	my_sleep(coder->sim->time_to_refactor, coder->sim);
	if (check_simulation_stop(coder->sim))
		return ;
}

static int	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->threads_ready_mutex);
	while (coder->sim->threads_ready == 0)
		pthread_cond_wait(&coder->sim->threads_ready_cond, &coder->sim->threads_ready_mutex);
	if (coder->sim->threads_ready == -1)
	{
		pthread_mutex_unlock(&coder->sim->threads_ready_mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->sim->threads_ready_mutex);
	return (1);
}

static int	has_reached_compile_limit(t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->lock_l_c_s);
	done = (coder->compile_count >= coder->sim->number_of_compiles_required);
	pthread_mutex_unlock(&coder->lock_l_c_s);
	return (done);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)arg;
	if (!wait_for_start(coder))
		return (NULL);
	if (coder->id % 2 == 0)
		my_sleep(coder->sim->time_to_compile / 2, coder->sim);
	while (1)
	{
		if (check_simulation_stop(coder->sim))
			break ;
		get_first_and_second_dongles(coder, &first, &second);
		if (!do_compile(coder, first, second))
			break ;
		do_debug_refactor(coder);
		if (has_reached_compile_limit(coder))
			break ;
	}
	return (NULL);
}
