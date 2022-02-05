#include <stdio.h>

void __attribute__((destructor)) destructor(void)
{
  printf("Hello, I`m destructor\n");
}


void __attribute__((constructor)) constructor(void)
{
  printf("Hello, I`m constructor\n");
}

