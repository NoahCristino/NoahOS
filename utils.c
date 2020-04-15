#include "utils.h"

uint32 strlen(const char* str)
{
  uint32 length = 0;
  while(str[length])
    length++;
  return length;
}

int strcmp(const char *p1, const char *p2)

{

  const unsigned char *s1 = (const unsigned char *) p1;

  const unsigned char *s2 = (const unsigned char *) p2;

  unsigned char c1, c2;

  do

    {

      c1 = (unsigned char) *s1++;

      c2 = (unsigned char) *s2++;

      if (c1 == '\0')

        return c1 - c2;

    }

  while (c1 == c2);

  return c1 - c2;

}
uint32 digit_count(int num)
{
  uint32 count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}

void itoa(int num, char *number)
{
  int dgcount = digit_count(num);
  int index = dgcount - 1;
  char x;
  if(num == 0 && dgcount == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[dgcount] = '\0';
  }
}




