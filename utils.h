#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define TRUE  1
#define FALSE 0

extern uint32 strlen(const char*);
extern uint32 digit_count(int);
extern int strcmp(const char*, const char*);
extern void itoa(int, char *);

#endif

