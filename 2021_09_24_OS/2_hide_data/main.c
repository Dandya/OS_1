#include <stdio.h>
#include "lib.h"

#define DEBUG fprintf(stderr,"LINE: %d\n", __LINE__)
int main(int argc, char *argv[])
{
  struct st st;
  DEBUG;
  my_init(&st);
  printf("a = %x, b= %x\n",st.a, st.b); 
  DEBUG;
  my_check(&st);
  DEBUG;
  st.a = 0;
  printf("a = %x, b= %x\n",st.a, st.b); 
  DEBUG;
  my_check(&st);
  DEBUG;
  return 0;
}
