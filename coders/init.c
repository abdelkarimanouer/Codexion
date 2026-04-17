/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 15:35:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/04/17 15:40:22 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_simulation_with_default_values(t_simulation **sim)
{
	(*sim)->number_of_coders = -1;
	(*sim)->time_to_compile = -1;
	(*sim)->time_to_debug = -1;
	(*sim)->time_to_refactor = -1;
	(*sim)->number_of_compiles_required = -1;
	(*sim)->dongle_cooldown = -1;
	(*sim)->scheduler = "";
}
