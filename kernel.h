#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#define VGA_ADDRESS 0xB8000
#define BUFSIZE 2200

uint16_t* vga_buffer;
extern uint32_t vga_index;

enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

extern void clear_screen(uint8_t, uint8_t);
extern void print_new_line();
extern void draw_banner();
extern void draw_title(char *);
extern void draw_login();
extern void draw_menu(int);
extern void programs();
extern void files();
extern void about();
extern void menu();
extern void login();
extern void print_char(char);
extern void print_string(char *);
extern void print_color_string(const char *, uint8_t, uint8_t);
extern void print_int(int);
extern uint8_t inb(uint16_t);
extern void outb(uint16_t, uint8_t);
extern uint8_t get_input_keycode();
extern void sleep(uint32_t);
extern void gotoxy(uint16_t, uint16_t);

#endif
