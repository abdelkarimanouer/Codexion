/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 07:51:36 by aanouer           #+#    #+#             */
/*   Updated: 2026/03/31 08:26:39 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	valid_args(t_simulation *sim)
{
	if (sim->number_of_coders == 0 || sim->time_to_burnout == 0
		|| sim->time_to_compile == 0 || sim->time_to_debug == 0
		|| sim->time_to_refactor == 0 || sim->number_of_compiles_required == 0
		|| sim->dongle_cooldown == 0
		|| (strcmp(sim->scheduler, "fifo") != 0
			&& strcmp(sim->scheduler, "edf") != 0)
	)
		return (0);
	else
		return (1);
}

int	parsing_args(t_simulation *sim, char **v)
{
	sim->number_of_coders = atoi(v[1]);
	sim->time_to_burnout = atoi(v[2]);
	sim->time_to_compile = atoi(v[3]);
	sim->time_to_debug = atoi(v[4]);
	sim->time_to_refactor = atoi(v[5]);
	sim->number_of_compiles_required = atoi(v[6]);
	sim->dongle_cooldown = atoi(v[7]);
	sim->scheduler = v[8];
	if (valid_args(sim))
		return (1);
	else
		return (0);
}
