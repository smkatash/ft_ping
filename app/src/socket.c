#include "ft_ping.h"



void create_raw_socket(t_ping *p) {
    p->raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (p->raw_socket == -1) {
        log_error(SOCKET_ERROR);
    }
}