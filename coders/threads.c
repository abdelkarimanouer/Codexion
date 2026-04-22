/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:21:58 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/22 10:22:17 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	rest_of_code(t_coder *coder)
{
	print_action(coder->sim, coder->id, "is debugging");
	my_sleep(coder->sim->time_to_debug, coder->sim);
	print_action(coder->sim, coder->id, "is refactoring");
	my_sleep(coder->sim->time_to_refactor, coder->sim);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (check_simulation_stop(coder->sim))
			break ;
		take_dongle(coder, coder->left_dongle);
		take_dongle(coder, coder->right_dongle);
		pthread_mutex_lock(&coder->lock_l_c_s);
		coder->last_compile_start = get_current_time();
		pthread_mutex_unlock(&coder->lock_l_c_s);
		print_action(coder->sim, coder->id, "is compiling");
		my_sleep(coder->sim->time_to_compile, coder->sim);
		release_dongle(coder, coder->right_dongle);
		release_dongle(coder, coder->left_dongle);
		coder->compile_count++;
		if (coder->compile_count >= coder->sim->number_of_compiles_required)
			break;
		else
			rest_of_code(coder);
	}
	return (NULL);
}
