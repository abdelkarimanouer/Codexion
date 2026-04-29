/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 11:02:55 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/29 10:31:12 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	get_first_and_second_dongles(t_coder *coder, t_dongle **first,
		t_dongle **second)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

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
	release_dongle(coder, first);
	release_dongle(coder, second);
	coder->compile_count++;
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
	pthread_mutex_lock(&coder->lock_l_c_s);
	coder->last_compile_start = get_current_time();
	pthread_mutex_unlock(&coder->lock_l_c_s);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (check_simulation_stop(coder->sim))
			break ;
		get_first_and_second_dongles(coder, &first, &second);
		if (!do_compile(coder, first, second))
			break ;
		do_debug_refactor(coder);
		if (coder->compile_count >= coder->sim->number_of_compiles_required)
			break ;
	}
	return (NULL);
}
