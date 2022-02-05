#include <mcheck.h>
#include <stdlib.h>

#define new(x) malloc((x))

int main(int argc, char *argv[])
{
  mtrace();
  char* ptr  = new(0x20);
  
  //free(ptr);
  return 0;
}
