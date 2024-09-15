#include "util.h"

/* standard funcs */
void generateRandomString(char *str, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t max_index = sizeof(charset) - 1;
    
    for (size_t i = 0; i < length; i++) {
        int key = rand() % max_index;
        str[i] = charset[key];
    }
    str[length] = '\0'; // Null-terminate the string
}

void die(const char *fmt, ...)
{
        va_list args;

        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);

        if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
                fputc(' ', stderr);
                perror(NULL);
        } else {
                fputc('\n', stderr);
        }

        exit(1);
}
