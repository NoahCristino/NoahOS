#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define TRUE  1
#define FALSE 0

extern uint32_t strlen(const char*);
extern uint32_t digit_count(int);
extern int strcmp(const char*, const char*);
extern void itoa(int, char *);

#endif

