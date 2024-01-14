/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:19:50 by kanykei           #+#    #+#             */
/*   Updated: 2024/01/14 20:22:23 by kanykei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_ping.h"

t_stats g_ping_stats;

static void    set_up_connection(t_ping *p) {
    create_raw_socket(p);
    get_hostname_address_info(p);
}

static void ping_loop(t_ping *p) {
    signal(SIGINT, log_stats);
    signal(SIGALRM, SIG_IGN);
    log_header(p);
    p->seq = 1;
    p->interval = 1;
    g_ping_stats.min = 0;
    g_ping_stats.max = 0;
    g_ping_stats.total = 0;
    timestamp(&(g_ping_stats.start));
    while (1) {
        ping(p);
    }
}


int main(int argc, char** argv) {
    t_ping p;

    // TODO Root check
    input_check(argc, argv, &p);
    set_up_connection(&p);
    ft_memset(&g_ping_stats, 0, sizeof(t_stats));
    ping_loop(&p);
}
