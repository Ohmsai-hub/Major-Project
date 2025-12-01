#ifndef UTILS_H
#define UTILS_H

#include <time.h>

/* Small I/O helpers */
void strip_newline(char *s);
void read_line(const char *prompt, char *buf, size_t sz);
double read_double(const char *prompt);

/* Print human readable time */
void print_time(time_t ts);

#endif /* UTILS_H */
