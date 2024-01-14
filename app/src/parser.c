#include "ft_ping.h"

void    input_check(int num, char **arr, t_ping *p) {
    if (num != 2) {
        log_error(WRONG_INPUT);
    }
    p->hostinfo.hostname = strdup(arr[1]);
}

void get_hostname_address_info(t_ping *p) {
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET,
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(p->hostinfo.hostname, NULL, &hints, &res) == 0) {
        ft_memcpy(&p->hostinfo.inet_addr, res->ai_addr, sizeof(struct sockaddr));
        inet_ntop(AF_INET, &(p->hostinfo.inet_addr.sin_addr), p->hostinfo.ip_addr, INET_ADDRSTRLEN);
    } else {
        log_error("gethostaddrinfo");
    }

}