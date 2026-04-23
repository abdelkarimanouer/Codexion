/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_part_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 06:23:59 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/23 11:04:39 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_coders(t_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = get_current_time();
		pthread_mutex_init(&sim->coders[i].lock_l_c_s, NULL);
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = (&sim->dongles[(i + 1)
				% sim->number_of_coders]);
		sim->coders[i].sim = sim;
		i++;
	}
}
