/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/04 15:51:15 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	initialize(t_simulation *sim, t_coder **coders,
			t_dongle **dongles)
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

void	free_clean_everything(t_simulation **sim, t_coder **coders,
			t_dongle **dongles)
{
	free(*sim);
	free(*coders);
	free(*dongles);
}

int	start(t_simulation **sim, t_coder **coders,
			t_dongle **dongles, char **argv)
{
	if (!parsing_args(*sim, argv))
	{
		free(*sim);
		return (1);
	}
	if (!initialize(*sim, coders, dongles))
	{
		free_clean_everything(sim, coders, dongles);
		return (1);
	}
	return 0;
}

int	main(int argc, char **argv)
{
	t_simulation	*sim;
	t_dongle		*dongles;
	t_coder			*coders;

	if (argc != 9)
	{
		fprintf(stderr, "[ERROR]: Number of Args should be Exactly 9 with program name!\n");
		return (1);
	}
	sim = malloc(sizeof(t_simulation));
	if (!sim)
	{
		fprintf(stderr, "[ERROR]: Memory allocation failed");
		return (1);
	}
	if (start(&sim, &coders, &dongles, argv))
	{
		fprintf(stderr, "[ERROR]: invalid Args or Memory allocation failed\n");
		return (1);
	}
	free_clean_everything(&sim, &coders, &dongles);
	return (0);
}
