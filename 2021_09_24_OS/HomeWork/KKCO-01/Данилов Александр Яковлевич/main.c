#include "smartmem.h"
//----------------------------------------------------------------
int main(int argc, char* argv[]) 
{
  void* ptr;
  for (int i = 0; i<100; i++)
  {
    ptr = new(1000);
    printf("%lld\n", ptr);
  }
  free(ptr);
}