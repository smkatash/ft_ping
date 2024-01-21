#include "ft_ping.h"


static void create_socket(t_ping *p) {
    p->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (p->socket == -1) {
        log_error(strerror(errno));
    }
}

static  void set_server_hostname(t_ping *p) {
    struct sockaddr_in sa;
    char buff[NI_MAXHOST];
    int ret;

    sa.sin_family = AF_INET;
    inet_pton(AF_INET, p->hostinfo.ip_addr, &(sa.sin_addr));
    ret = getnameinfo((struct sockaddr *)&sa, sizeof(sa), buff, 
            sizeof(buff), NULL, 0, NI_NAMEREQD);
    if (ret < 0) {
        close(p->socket);
        log_error(gai_strerror(ret));                      
    }
    p->hostinfo.server_hostname = ft_strdup(buff);
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
        p->hostinfo.hostname = ft_strdup(res->ai_canonname);
        inet_ntop(AF_INET, &(p->hostinfo.inet_addr.sin_addr), p->hostinfo.ip_addr, INET_ADDRSTRLEN);
    } else {
        close(p->socket);
        freeaddrinfo(res);
        log_error(gai_strerror(ret));
    }
    freeaddrinfo(res);
}

void    init_ping(t_ping *p) {
    create_socket(p);
    set_hostaddress(p);
    set_server_hostname(p);
}