#ifndef BOX_H
#define BOX_H


#include <stdint.h>
#define BOX_MAX_WIDTH 78
#define BOX_MAX_HEIGHT 23

#define BOX_SINGLELINE 1
#define BOX_DOUBLELINE 2



extern void draw_generic_box(uint16_t, uint16_t, uint16_t, uint16_t, 
                            uint8_t, uint8_t, uint8_t, uint8_t, 
                            uint8_t, uint8_t, uint8_t, uint8_t);

extern void draw_box(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t,
                      uint8_t, uint8_t);

extern void fill_box(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);


#endif


