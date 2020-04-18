#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

extern void gdt_set_gate(uint8_t, unsigned long, unsigned long, unsigned char, unsigned char); //int -> uint8_t?
extern void gdt_install();
#endif
