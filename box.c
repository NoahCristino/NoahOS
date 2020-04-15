#include "kernel.h"
#include <stdint.h>
#include "utils.h"
#include "box.h"

uint16_t get_box_draw_char(uint8_t chn, uint8_t fore_color, uint8_t back_color)
{
  uint16_t ax = 0;
  uint8_t ah = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  ax |= chn;

  return ax;
}

void draw_generic_box(uint16_t x, uint16_t y, 
                      uint16_t width, uint16_t height,
                      uint8_t fore_color, uint8_t back_color,
                      uint8_t topleft_ch,
                      uint8_t topbottom_ch,
                      uint8_t topright_ch,
                      uint8_t leftrightside_ch,
                      uint8_t bottomleft_ch,
                      uint8_t bottomright_ch)
{
  uint32_t i;

  //increase vga_index to x & y location
  vga_index = 80*y;
  vga_index += x;

  //draw top-left box character
  vga_buffer[vga_index] = get_box_draw_char(topleft_ch, fore_color, back_color);

  vga_index++;
  //draw box top characters, -
  for(i = 0; i < width; i++){
    vga_buffer[vga_index] = get_box_draw_char(topbottom_ch, fore_color, back_color);
    vga_index++;
  }

  //draw top-right box character
  vga_buffer[vga_index] = get_box_draw_char(topright_ch, fore_color, back_color);

  // increase y, for drawing next line
  y++;
  // goto next line
  vga_index = 80*y;
  vga_index += x;

  //draw left and right sides of box
  for(i = 0; i < height; i++){
    //draw left side character
    vga_buffer[vga_index] = get_box_draw_char(leftrightside_ch, fore_color, back_color);
    vga_index++;
    //increase vga_index to the width of box
    vga_index += width;
    //draw right side character
    vga_buffer[vga_index] = get_box_draw_char(leftrightside_ch, fore_color, back_color);
    //goto next line
    y++;
    vga_index = 80*y;
    vga_index += x;
  }
  //draw bottom-left box character
  vga_buffer[vga_index] = get_box_draw_char(bottomleft_ch, fore_color, back_color);
  vga_index++;
  //draw box bottom characters, -
  for(i = 0; i < width; i++){
    vga_buffer[vga_index] = get_box_draw_char(topbottom_ch, fore_color, back_color);
    vga_index++;
  }
  //draw bottom-right box character
  vga_buffer[vga_index] = get_box_draw_char(bottomright_ch, fore_color, back_color);

  vga_index = 0;
}

void draw_box(uint8_t boxtype, 
              uint16_t x, uint16_t y, 
              uint16_t width, uint16_t height,
              uint8_t fore_color, uint8_t back_color)
{
  switch(boxtype){
    case BOX_SINGLELINE : 
      draw_generic_box(x, y, width, height, 
                      fore_color, back_color, 
                      218, 196, 191, 179, 192, 217);
      break;

    case BOX_DOUBLELINE : 
      draw_generic_box(x, y, width, height, 
                      fore_color, back_color, 
                      201, 205, 187, 186, 200, 188);
      break;
  }
}

void fill_box(uint8_t ch, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color)
{
  uint32_t i,j;

  for(i = 0; i < height; i++){
    //increase vga_index to x & y location
    vga_index = 80*y;
    vga_index += x;

    for(j = 0; j < width; j++){
      vga_buffer[vga_index] = get_box_draw_char(ch, 0, color);
      vga_index++;
    }
    y++;
  }
}


