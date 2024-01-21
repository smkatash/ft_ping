/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 11:19:58 by kanykei           #+#    #+#             */
/*   Updated: 2024/01/21 21:18:27 by kanykei          ###   ########.fr       */
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
# include <limits.h>

# define WRONG_INPUT "usage error: Destination address required"
# define SOCKET_ERROR "Failed to create a socket"
# define WRONG_IP_PROTOCOL "Received wrong ip protocol"
# define TIME_EXCEEDED "ttl exceeded"
# define HOST_ERROR "host error"


// ip header (20) + icmp header (8) + data (56) = 74 bytes.
#define IP_TTL_VALUE 64
#define IP_HDR_SIZE (sizeof(struct iphdr))
#define ICMP_HDR_SIZE (sizeof(struct icmphdr)) 
#define ICMP_DATA_SIZE 56
#define MAX_PACKET_SIZE 1500

/* ICMP Header:
- Type (1 byte)
- Code (1 byte)
- Checksum (2 bytes)
- Identifier (2 bytes)
- Sequence Number (2 bytes) 
*/

typedef struct s_host {
    char    *hostname;
    char    *server_hostname;
    struct  sockaddr_in inet_addr;
    char    ip_addr[INET_ADDRSTRLEN];
} t_host;

typedef struct s_options {
    int    verbose;
} t_options;

typedef struct s_ping {
    int             socket;
    t_host          hostinfo;
    t_options       opts;
    int             seq;
    uint16_t        cpid;
    double          interval;
    struct timeval  start;
    struct timeval  start_rtt;
    struct timeval  end_rtt;
    double          rtt;
    int             sent;
    int             received;
    double          min;
    double          max;
    double          sum;
    double          sum2;
} t_ping;

extern int ping_end;

void            parse_input(int argc, char **argv, t_ping *p);
void            init_stats(t_ping *p);
void            init_ping(t_ping *p);
void            ping(t_ping *p);
int             valid_response(void *resp, t_ping *p); 
void            wsleep(t_ping *p);
double          get_round_trip_time(struct timeval start, struct timeval end);
void            save_time_stats(t_ping *p);
double          double_sqrt(double a);
unsigned short  checksum(void *header, int len);
void            log_error(const char *err);
void            log_info(const char *str);
void            log_header(t_ping *p);
void            log_stats(t_ping *p);
void            log_response(t_ping *p, void *msg, int bytes);
void            log_help();





#endif