/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/17 15:17:43 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_clean_everything(t_simulation **sim)
{
	if (!sim || !*sim)
		return ;
	free(*sim);
	*sim = NULL;
}

int	start(t_simulation **sim, char **argv)
{
	if (!parsing_args(*sim, argv))
	{
		free(*sim);
		return (1);
	}
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
	if (start(&sim, argv))
		return (fprintf(stderr, "[ERROR]: bad_args or mem_alloc_fail\n"), 1);

	return (free_clean_everything(&sim), 0);
}
