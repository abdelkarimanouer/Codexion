/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/05/08 11:39:42 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_initialize(t_simulation **sim)
{
	if (!init_dongles_and_coders(sim))
		return (free(*sim), 1);
	if (!init_queue_of_dongles(&(*sim)->dongles, (*sim)->number_of_coders))
	{
		free((*sim)->coders);
		free((*sim)->dongles);
		free(*sim);
		return (1);
	}
	if (!init_mutexes_and_dongles(*sim))
		return (free_clean_everything(sim), 1);
	if (!init_coders_with_info(*sim))
		return (free_clean_everything(sim), 1);
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
	if (!parsing_args(sim, argv))
		return (free(sim), 1);
	if (start_initialize(&sim))
		return (fprintf(stderr, "[ERROR]: bad_args or mem_alloc_fail\n"), 1);
	if (start_threads(sim))
	{
		free_clean_everything(&sim);
		return (1);
	}
	join_threads(sim);
	return (free_clean_everything(&sim), 0);
}
