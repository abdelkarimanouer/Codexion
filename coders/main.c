/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanouer <aanouer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:56:20 by aanouer           #+#    #+#             */
/*   Updated: 2026/03/27 16:55:21 by aanouer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <pthread.h>

int	main(int argc, char **argv)
{
	if (argc == 9)
	{
	}
	else
	{
		write(2, "Invalid Args\n", 13);
		return (1);
	}
	return (0);
}
