#include "ft_ping.h"


static void create_socket(t_ping *p) {
    int ttl = IP_TTL_VALUE;
    struct timeval recvtimeout;
    recvtimeout.tv_sec = RECV_TIMEOUT_SEC;
    recvtimeout.tv_usec = 0;

    if ((p->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        log_error(strerror(errno));
    }
    if (setsockopt(p->socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1) {
        close(p->socket);
        log_error(strerror(errno));
    }

    if (setsockopt(p->socket, SOL_SOCKET, SO_RCVTIMEO, &recvtimeout, sizeof(recvtimeout)) == -1) {
        close(p->socket);
        log_error(strerror(errno));
    }
}

static  void set_server_hostname(t_ping *p) {
    struct sockaddr_in sa;
    char buff[NI_MAXHOST];
    int ret;

    sa.sin_family = AF_INET;
    inet_pton(AF_INET, p->hostinfo.ip_addr, &(sa.sin_addr));
    if ((ret = getnameinfo((struct sockaddr *)&sa, sizeof(sa), buff, 
            sizeof(buff), NULL, 0, NI_NAMEREQD)) == 0) {
        p->hostinfo.server_hostname = ft_strdup(buff);
    } else if (ret == -1) {
        free_ping(p);
        log_error(gai_strerror(ret));                      
    }
}

static void set_hostaddress(t_ping *p) {
    struct addrinfo hints;
    struct addrinfo *res;
    int ret;

    ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET,
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = AI_CANONNAME;
    ret = getaddrinfo(p->hostinfo.hostname, NULL, &hints, &res);
    if (ret == 0) {
        ft_memcpy(&p->hostinfo.inet_addr, res->ai_addr, sizeof(struct sockaddr));
        if (ft_strcmp(p->hostinfo.hostname, res->ai_canonname) != 0) {
            free(p->hostinfo.hostname);
            p->hostinfo.hostname = ft_strdup(res->ai_canonname);
        }
        inet_ntop(AF_INET, &(p->hostinfo.inet_addr.sin_addr), p->hostinfo.ip_addr, INET_ADDRSTRLEN);
    } else {
        free_ping(p);
        log_error(gai_strerror(ret));
    }
    freeaddrinfo(res);
}

void    init_ping(t_ping *p) {
    init_stats(p);
    create_socket(p);
    set_hostaddress(p);
    set_server_hostname(p);
}