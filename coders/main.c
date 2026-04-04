/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/04 14:56:56 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	initialize(t_simulation *sim, t_coder **coders, t_dongle **dongles)
{
	initialize_mutexes(sim);
	*dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!*dongles)
		return (0);
	sim->dongles = *dongles;
	initialize_dongles(sim, *dongles);
	*coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!*coders)
		return (0);
	sim->coders = *coders;
	initialize_coders(sim, *coders, *dongles);
	return (1);
}

int	main(int argc, char **argv)
{
	t_simulation	*sim;
	t_dongle		*dongles;
	t_coder			*coders;

	if (argc != 9)
	{
		write(2, "Invalid Args\n", 13);
		return (1);
	}
	sim = malloc(sizeof(t_simulation));
	if (!sim)
	{
		write(2, "Invalid Args\n", 13);
		return (1);
	}
	if (!parsing_args(sim, argv) || !initialize(sim, &coders, &dongles))
	{
		write(2, "Invalid Args\n", 13);
		return (1);
	}
	return (0);
}
