#include "logger.h"

#include <stdio.h>

void print_log(const char *log);

void log_message(const char *message)
{
    print_log(message);
    fflush(stdout);
}

void log_warning(const char *warning)
{
    printf("\033[33m");
    print_log(warning);
    printf("\033[m");
    fflush(stdout);
}

void log_error(const char *error)
{
    printf("\033[31m");
    print_log(error);
    printf("\033[m");
    fflush(stdout);
}

void print_log(const char *log)
{
    printf("[%s]\n", log);
}