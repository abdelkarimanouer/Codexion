/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 07:51:36 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/03 13:30:40 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_args(t_simulation *sim)
{
	if (sim->number_of_coders <= 0 || sim->time_to_burnout < 0
		|| sim->time_to_compile < 0 || sim->time_to_debug < 0
		|| sim->time_to_refactor < 0 || sim->number_of_compiles_required <= 0
		|| sim->dongle_cooldown < 0 || (strcmp(sim->scheduler, "fifo") != 0
			&& strcmp(sim->scheduler, "edf") != 0))
		return (0);
	else
		return (1);
}

int	parsing_args(t_simulation *sim, char **v)
{
	int	faild;

	faild = 0;
	sim->number_of_coders = ft_atoi(v[1], &faild);
	if (faild)
		return (faild = 0, 0);
	sim->time_to_burnout = ft_atoi(v[2], &faild);
	if (faild)
		return (faild = 0, 0);
	sim->time_to_compile = ft_atoi(v[3], &faild);
	if (faild)
		return (faild = 0, 0);
	sim->time_to_debug = ft_atoi(v[4], &faild);
	if (faild)
		return (faild = 0, 0);
	sim->time_to_refactor = ft_atoi(v[5], &faild);
	if (faild)
		return (faild = 0, 0);
	sim->number_of_compiles_required = ft_atoi(v[6], &faild);
	if (faild)
		return (faild = 0, 0);
	sim->dongle_cooldown = ft_atoi(v[7], &faild);
	if (faild)
		return (faild = 0, 0);
	sim->scheduler = v[8];
	if (faild)
		return (0);
	if (check_args(sim))
		return (1);
	else
		return (0);
}
