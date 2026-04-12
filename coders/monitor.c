/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:32:03 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/12 14:16:38 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	check_if_coders_compiled_enough(t_simulation *sim, int *i)
{
	while (*i < sim->number_of_coders)
	{
		if (sim->coders[*i].compile_count < sim->number_of_compiles_required)
			break ;
		(*i)++;
	}
	if (*i == sim->number_of_coders)
		sim->stop_simulation = 1;
}

static long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000000LL + tv.tv_usec);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;
	long long		current_time;

	sim = (t_simulation *)arg;
	while (sim->stop_simulation == 0)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			current_time = get_time();
			sim->t_s_comp = current_time - sim->coders[i].last_compile_time;
			if (sim->t_s_comp > (sim->time_to_burnout * 1000))
			{
				log_action(sim, sim->coders[i].id, "burned out");
				sim->stop_simulation = 1;
				break ;
			}
			i++;
		}
		i = 0;
		check_if_coders_compiled_enough(sim, &i);
		usleep(1000);
	}
	return (NULL);
}
