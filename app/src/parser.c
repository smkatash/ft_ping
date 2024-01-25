# include "ft_ping.h"


void    parse_destaddress(char *str, t_ping *p) {
    if (ft_strlen(str) > 1 && str[0] == '-') {
        log_error(USAGE_ERROR);
    }
    p->hostinfo.hostname = ft_strdup(str);
}

bool    str_isdigit(char *str) {
    for (size_t i = 0; i < ft_strlen(str); i++) {
        if (!ft_isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int    parse_number(char *str) {
    int num;

    if (!str_isdigit(str)) {
            // TODO find better way to log and free 
            dprintf(STDERR_FILENO, "ft_ping invalid argument: '%s'", str);
            exit(EXIT_FAILURE);
    }
    num = ft_atoi(str);
    if (num <= 0) {
        // TODO find better way to log and free 
        dprintf(STDERR_FILENO, "ft_ping invalid argument: '%s' Numerical result out of range\n", str);
        exit(EXIT_FAILURE);
    }
    return num;
}

void    parse_options(int argc, char **argv, t_ping *p) {
    for (int i = 1; i < argc; i++) {
        if (ft_strcmp(argv[i], HELP) == 0) {
            log_help();
        } else if (ft_strcmp(argv[i], VERBOSE) == 0) {
            p->opts.verbose = 1;
        } else if (ft_strcmp(argv[i], QUIET) == 0) {
            p->opts.quiet = 1;
        } else if (ft_strcmp(argv[i], COUNT) == 0) {
            ++i;
            if (argv[i]) {
                p->opts.count = parse_number(argv[i]);
                continue;
            }
            log_help();
        } else if (ft_strcmp(argv[i], WAIT) == 0) {
            ++i;
            if (argv[i]) {
                p->opts.wait = parse_number(argv[i]);
                continue;
            }
            log_help();
        } else {
            parse_destaddress(argv[i], p);
        }        
    }
}

void    parse_input(int argc, char **argv, t_ping *p) {
    if (argc < 2) {
        log_error(USAGE_ERROR);
    }
    ft_bzero(&(p->opts), sizeof(t_options));
    parse_options(argc,argv,p);
    if (!p->hostinfo.hostname) {
        log_error(USAGE_ERROR);
    }
}