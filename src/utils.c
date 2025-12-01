#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

void strip_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[n-1] = '\0';
        --n;
    }
}

void read_line(const char *prompt, char *buf, size_t sz) {
    if (prompt) printf("%s", prompt);
    if (fgets(buf, (int)sz, stdin) == NULL) {
        clearerr(stdin);
        buf[0] = '\0';
    }
    strip_newline(buf);
}

double read_double(const char *prompt) {
    char buf[128];
    double v;
    while (1) {
        read_line(prompt, buf, sizeof(buf));
        if (sscanf(buf, "%lf", &v) == 1) return v;
        printf("Invalid number, try again.\n");
    }
}

void print_time(time_t ts) {
    char buf[64];
    struct tm *tm = localtime(&ts);
    if (tm && strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm)) {
        printf("%s", buf);
    } else {
        printf("%ld", (long)ts);
    }
}

