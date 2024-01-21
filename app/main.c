/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:19:50 by kanykei           #+#    #+#             */
/*   Updated: 2024/01/21 20:09:08 by kanykei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

int ping_end = 0;

static void sighandler(int sig) {
    if (sig == SIGINT) {
        ping_end = 1;
    } 
}

static void ping_loop(t_ping *p) {
    signal(SIGINT, &sighandler);
    signal(SIGALRM, SIG_IGN);
    log_header(p);
    if (gettimeofday(&(p->start), NULL) == -1) {
        log_error(strerror(errno));
    }
    while (1) {
        ping(p);
        wsleep(p);
        if (ping_end) {
            log_stats(p);
        }
    }
}

int main(int argc, char** argv) {
    t_ping p;

    ft_memset(&p, 0, sizeof(t_ping));
    parse_input(argc, argv, &p);
    init_stats(&p);
    init_ping(&p);
    ping_loop(&p);
}
