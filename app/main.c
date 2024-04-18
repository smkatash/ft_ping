/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:19:50 by kanykei           #+#    #+#             */
/*   Updated: 2024/04/18 15:03:45 by kanykei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int	g_ping_end = 0;

static void	sighandler(int sig)
{
	if (sig == SIGINT)
	{
		g_ping_end = 1;
	}
}

static void	ping_loop(t_ping *p)
{
	signal(SIGINT, &sighandler);
	log_header(p);
	if (gettimeofday(&(p->start), NULL) == -1)
	{
		free_ping(p);
		log_error(strerror(errno));
	}
	while (1)
	{
		ping(p);
		wsleep(p);
		if (g_ping_end || (p->opts.count > 0 && p->opts.count == p->sent))
		{
			log_stats(p);
			return ;
		}
	}
}

int	main(int argc, char **argv)
{
	t_ping	p;

	ft_memset(&p, 0, sizeof(t_ping));
	parse_input(argc, argv, &p);
	init_ping(&p);
	ping_loop(&p);
	free_ping(&p);
}
