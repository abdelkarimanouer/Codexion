/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:32:03 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/10 12:03:51 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;
	struct timeval	tv;
	long long		current_time;

	sim = (t_simulation *)arg;
	i = 0;
	while (sim->stop_simulation == 0)
	{
		while (i < sim->number_of_coders)
		{
			gettimeofday(&tv, NULL);
			current_time = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
			sim->time_since_compile = current_time - sim->coders[i].last_compile_time;
		}
	}
}
