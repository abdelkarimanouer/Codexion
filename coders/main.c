/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/24 11:00:28 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start(t_simulation **sim, char **argv)
{
	if (!parsing_args(*sim, argv))
		return (free(*sim), 1);
	if (!init_dongles_and_coders(sim))
		return (free(*sim), 1);
	if (!init_queue_of_dongles(&(*sim)->dongles,
			(*sim)->number_of_coders))
	{
		free((*sim)->coders);
		free((*sim)->dongles);
		free(*sim);
		return (1);
	}
	init_mutexes_and_dongles(*sim);
	init_coders(*sim);
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	*sim;

	if (argc != 9)
		return (fprintf(stderr,
				"[ERROR]: Number of Args should be Exactly "
				"9 with program name!\n"), 1);
	sim = malloc(sizeof(t_simulation));
	if (!sim)
		return (fprintf(stderr, "[ERROR]: Memory allocation failed"), 1);
	init_simulation_with_default_values(&sim);
	if (start(&sim, argv))
		return (fprintf(stderr, "[ERROR]: bad_args or mem_alloc_fail\n"), 1);
	sim->start_timestamp = get_current_time();
	if (start_threads(sim))
	{
		free_clean_everything(&sim);
		return (1);
	}
	join_threads(sim);
	return (free_clean_everything(&sim), 0);
}
