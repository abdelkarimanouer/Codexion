/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/03/31 10:36:43 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	*sim;
	t_dongle		*dongle;
	t_coder			*coder;

	if (argc != 9)
	{
		write(2, "Invalid Args\n", 13);
		return (1);
	}
	sim = malloc(sizeof(t_simulation));
	if (!parsing_args(sim, argv))
	{
		write(2, "Invalid Args\n", 13);
		return (1);
	}
	return (0);
}
