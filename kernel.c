#include "kernel.h"
#include "utils.h"
#include "keyboard.h"
#include "char.h"
#include "box.h"
#include "gdt.h"
uint32_t vga_index;
static uint32_t next_line_index = 1;
uint8_t g_fore_color = WHITE, g_back_color = BLUE;
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

uint16_t vga_entry(unsigned char ch, uint8_t fore_color, uint8_t back_color) 
{
  uint16_t ax = 0;
  uint8_t ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

void clear_vga_buffer(uint16_t **buffer, uint8_t fore_color, uint8_t back_color)
{
  uint32_t i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(0, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}

void init_vga(uint8_t fore_color, uint8_t back_color)
{ 
  vga_buffer = (uint16_t*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

void clear_screen(uint8_t fore_color, uint8_t back_color)
{
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
}

void print_new_line()
{
  if(next_line_index >= 55){
    next_line_index = 0;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
  }
  vga_index = 80*next_line_index;
  next_line_index++;
}

void print_char(char ch)
{
  vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
  vga_index++;
}

void print_string(char *str)
{
  uint32_t index = 0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

void print_color_string(const char* str, uint8_t fore_color, uint8_t back_color)
{
  uint32_t index = 0;
  uint8_t fc, bc;
  fc = g_fore_color;
  bc = g_back_color;
  g_fore_color = fore_color;
  g_back_color = back_color;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
  g_fore_color = fc;
  g_back_color = bc;
}

void print_int(int num)
{
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_string(str_num);
}

uint8_t inb(uint16_t port)
{
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}
void outb(uint16_t port, uint8_t data) {
  asm("outb %%al, %%dx" : : "a"(data), "d"(port));
}


uint8_t get_input_keycode()
{
  uint8_t keycode = 0;
  while((keycode = inb(KEYBOARD_PORT)) != 0){
    if(keycode > 0)
      return keycode;
  }
  return keycode;
}

/*
keep the cpu busy for doing nothing(nop)
so that io port will not be processed by cpu
here timer can also be used, but lets do this in looping counter
*/
void wait_for_io(uint32_t timer_count)
{
  while(1){
    asm volatile("nop");
    timer_count--;
    if(timer_count <= 0)
      break;
    }
}

void sleep(uint32_t timer_count)
{
  wait_for_io(timer_count);
}

void gotoxy(uint16_t x, uint16_t y)
{
  vga_index = 80*y;
  vga_index += x;
}
void draw_banner(){
  gotoxy(35,1);
  print_color_string("NoahOS V1.32", BRIGHT_RED, BLACK);
  draw_box(BOX_SINGLELINE, 34, 0, 11, 1, GREY, BLACK);
}
void draw_title(char *str){
  draw_box(BOX_SINGLELINE, 1, 3, 76, 20, GREY, BLACK);
  gotoxy(5,3);
  print_color_string(str,BRIGHT_RED,BLACK);
}
void draw_login(){
  clear_screen(WHITE,BLACK);
  draw_banner();
  draw_title("Login");
  gotoxy(10,8);
  print_color_string("Username:",BRIGHT_RED,BLACK);
  draw_box(BOX_SINGLELINE, 9, 7, 9, 1, GREY, BLACK);
  draw_box(BOX_SINGLELINE, 27, 7, 20, 1, GREY, BLACK);
  gotoxy(10,12);
  print_color_string("Password:",BRIGHT_RED,BLACK);
  draw_box(BOX_SINGLELINE, 9, 11, 9, 1, GREY, BLACK);
  draw_box(BOX_SINGLELINE, 27, 11, 20, 1, GREY, BLACK);

}
void draw_menu(int selection)
{
  clear_screen(WHITE,BLACK);
  draw_banner();
  draw_title("Menu");
  if (selection==1){
	  gotoxy(35,8);
	  print_color_string("--Programs--", BRIGHT_RED, BLACK);
	  draw_box(BOX_SINGLELINE, 34, 7, 11, 1, GREY, GREY);
  } else {
	  gotoxy(35,8);
	  print_color_string("--Programs--", BRIGHT_RED, BLACK);
	  draw_box(BOX_SINGLELINE, 34, 7, 11, 1, GREY, BLACK);
  }
  if (selection==2){
	  gotoxy(35,12);
	  print_color_string("---Files----", BRIGHT_RED, BLACK);
	  draw_box(BOX_SINGLELINE, 34, 11, 11, 1, GREY, GREY);
  } else {
       	  gotoxy(35,12);
          print_color_string("---Files----", BRIGHT_RED, BLACK);
          draw_box(BOX_SINGLELINE, 34, 11, 11, 1, GREY, BLACK);
  } 
  if (selection==3) {
	  gotoxy(35,16);
	  print_color_string("---About----", BRIGHT_RED, BLACK);
	  draw_box(BOX_SINGLELINE, 34, 15, 11, 1, GREY, GREY);
  } else {
	  gotoxy(35,16);
          print_color_string("---About----", BRIGHT_RED, BLACK);
          draw_box(BOX_SINGLELINE, 34, 15, 11, 1, GREY, BLACK);

  }
}
void programs(){
  clear_screen(WHITE,BLACK);
  draw_banner();
  draw_title("Programs");
  char keycode=0;
  bool con=0;
  do{

    keycode = get_input_keycode();
    if(keycode == KEY_BACKSPACE){
      con=1;
      menu();
    }
    sleep(0x39FFFFFF); //for live build
    //sleep(0x02FFFFFF); //for qemu
  }while(con == 0);

}
void files(){
  clear_screen(WHITE,BLACK);
  draw_banner();
  draw_title("Files");
  char keycode=0;
  bool con=0;
  do{

    keycode = get_input_keycode();
    if(keycode == KEY_BACKSPACE){
      con=1;
      menu();
    }
    sleep(0x39FFFFFF); //for live build
    //sleep(0x02FFFFFF); //for qemu
  }while(con == 0);
}
void about(){
  clear_screen(WHITE,BLACK);
  draw_banner();
  draw_title("About");
  gotoxy(10,8);
  print_string("This Operating System was developed by Noah Cristino");
  char keycode=0;
  bool con=0;
  do{

    keycode = get_input_keycode();
    if(keycode == KEY_BACKSPACE){
      con=1;
      menu();
    }
    sleep(0x39FFFFFF); //for live build
    //sleep(0x02FFFFFF); //for qemu
  }while(con == 0);}
void menu()
{
  
  int sel=1;
  int bpout=0;
  draw_menu(sel);
  bool selected=0;
  char keycode=0;
  sleep(0x39FFFFFF); //for live build
   // sleep(0x02FFFFFF); //for qemu
  do{

    keycode = get_input_keycode();
    if(keycode == KEY_ENTER){
      selected=1;
    }
    if(keycode == KEY_UP){
      if (sel>1){
        sel--;
	draw_menu(sel);
      }
    }
    if(keycode == KEY_DOWN){
      if (sel<3) {
        sel++;
	draw_menu(sel);
      }
    }
    if(keycode==KEY_BACKSPACE){
      if (bpout>0){
        login();
      } else {
        bpout++;
      }
    }
    sleep(0x39FFFFFF); //for live build
    //sleep(0x02FFFFFF); //for qemu

  }while(selected == 0);
  if (sel==1) {
    programs();
  }
  if (sel==2) {
    files();
  }
  if (sel==3) {
    about();
  }
}
void login()
{
  draw_login(); 
  char ch = 0;
  int inbox = 1;
  char keycode = 0;
  char pass[21];
  char username[21];
  int usrctr=0;
  int pasctr=0;
  bool allowenter=0;
  int linelen=0;
  gotoxy(28,8);
  do{

    keycode = get_input_keycode();

    if(keycode == KEY_ENTER){
      if (allowenter==1) {
        linelen=0;
        allowenter=0;
        inbox++;
        gotoxy(28,12);
        //print_color_string(username,GREEN,BLACK);
      }
    } else if (keycode == KEY_BACKSPACE) {
    	if (inbox<2) {
	  if (strlen(username)>0){
	    username[usrctr-1]=0;
	    username[usrctr]=0;
	    usrctr--;
	    draw_login();
	    gotoxy(28,8);
	    print_string(username);
	  }
	} else {
	   if (strlen(pass)>0){
            pass[pasctr-1]=0;
            pass[pasctr]=0;
	    pasctr--;
	    draw_login();
	    gotoxy(28,8);
	    print_string(username);
	    gotoxy(28,12);
	    for (uint32_t x=0;x!=strlen(pass);++x){
	    	print_char('*');
	    }
          }
	}
    }else{
      if (linelen<20) {
      ch = get_ascii_char(keycode);
      allowenter=1;
      if (inbox<2) {
	print_char(ch);
        username[usrctr]=ch;
        username[usrctr+1]='\0';
        usrctr++;
      }else{
	print_char('*');
        pass[pasctr]=ch;
        pass[pasctr+1]='\0';
        pasctr++;
      }
    }
    linelen++;
    }
    sleep(0x39FFFFFF); //for live build
    //sleep(0x02FFFFFF); //for qemu

  }while(inbox<3);
  gotoxy(28,5);
  //char* username2=username;
  //char* pass2=pass;
  if (strcmp(username,"NOAH")==0 && strcmp(pass,"ADMIN")==0) {
    print_color_string("Login Success!",GREEN,BLACK);
    sleep(0x80FFFFFF); //live
    //sleep(0x04FFFFFF); //qemu
    clear_screen(BLACK,BLACK);
    menu();
  } else {
    print_color_string("Login Invalid!",RED,BLACK);
    sleep(0x80FFFFFF); //live
    //sleep(0x04FFFFFF); //qemu
    clear_screen(BLACK,BLACK);
    login();
  }
}
void kernel_entry()
{
  gdt_install();
  init_vga(WHITE, BLACK);
  login();
}

