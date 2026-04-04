/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/04 16:59:13 by aanouer          ###   ########.fr       */
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
	int	i;

	if (!*sim)
		return ;
	i = 0;
	while (i < (*sim)->number_of_coders && *dongles)
	{
		pthread_mutex_destroy(&(*dongles)[i].mutex);
		pthread_cond_destroy(&(*dongles)[i].condition);
		i += 1;
	}
	pthread_mutex_destroy(&(*sim)->print_mutex);
	pthread_mutex_destroy(&(*sim)->stop_mutex);
	free(*sim);
	free(*coders);
	free(*dongles);
}

int	start(t_simulation **sim, t_coder **coders,
			t_dongle **dongles, char **argv)
{
	*coders = NULL;
	*dongles = NULL;
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
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	*sim;
	t_dongle		*dongles;
	t_coder			*coders;

	if (argc != 9)
		return (fprintf(stderr, "[ERROR]: Number of Args should be Exactly "
				"9 with program name!\n"), 1);
	sim = malloc(sizeof(t_simulation));
	if (!sim)
		return (fprintf(stderr, "[ERROR]: Memory allocation failed"), 1);
	if (start(&sim, &coders, &dongles, argv))
		return (fprintf(stderr, "[ERROR]: bad_args or mem_alloc_fail\n"), 1);
	return (free_clean_everything(&sim, &coders, &dongles), 0);
}
