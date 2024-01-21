# include "ft_ping.h"

void    parse_input(int argc, char **argv, t_ping *p) {

    if (argc < 2 && argc > 3) {
        log_error(WRONG_INPUT);
    }
    p->opts.verbose = 0;
    for (int i = 1; i < argc; i++) {
        if (ft_strcmp(argv[i], "-?") == 0) {
            log_help();
        } else if (ft_strcmp(argv[i], "-v") == 0) {
            p->opts.verbose = 1;
        } else {
            if (argv[i][0] == '-') {
                log_error(WRONG_INPUT);
            }
            p->hostinfo.hostname = ft_strdup(argv[i]); 
        }
    }
    if (!p->hostinfo.hostname) {
        log_error(WRONG_INPUT);
    }
}