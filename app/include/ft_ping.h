/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:19:58 by kanykei           #+#    #+#             */
/*   Updated: 2024/01/14 20:18:44 by kanykei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FT_PING_H
#define FT_PING_H

# include "../libs/libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <errno.h>
# include <math.h>

#define WRONG_INPUT "Error: Wrong input. Please try: ./ft_ping address/hostname"
#define SOCKET_ERROR "Error: Failed to create a socket"

typedef struct s_host {
    char    *hostname;
    struct  sockaddr_in inet_addr;
    char    ip_addr[INET_ADDRSTRLEN];
} t_host;

typedef struct s_ping {
    int             raw_socket;
    t_host          hostinfo;
    int             seq;
    uint16_t        cpid;
    double          interval;
    struct timeval  start_time;
    struct timeval  end_time;
} t_ping;

typedef struct s_stats {
    char            *hostname;
    int             sent;
    int             received;
    struct timeval  start;
    double          min;
    double          max;
    double          total;
} t_stats;

typedef struct		s_packet {
	struct icmp     icmp_header;
	char			payload[36];
}   t_packet;

typedef struct  s_response {
    struct msghdr   msg_header;
    struct iovec    iov;
    char            msg[84];
}   t_response;

extern  t_stats g_ping_stats;

void        input_check(int num, char **arr, t_ping *p);
void        create_raw_socket(t_ping *p);
void        get_hostname_address_info(t_ping *p);
void        ping(t_ping *p);

void        log_error(char *err);
void        log_info(char *str);
void        log_header(t_ping *p);
void        log_response(t_ping *p, t_response *resp, int bytes, uint16_t seq);
void        log_stats(int sig);

void        timestamp(struct timeval *tv);
void        wait_interval(t_ping *p);
double      get_round_trip_time(struct timeval start, struct timeval end);
void        save_time_stats(double time);
double      mdev();






#endif