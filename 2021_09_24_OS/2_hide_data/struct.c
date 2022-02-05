#include <stdio.h>

struct st {
  char a;
  int b;
}__attribute__((packed));

int main(int argc, char *argv[])
{
  printf("%d + %d = %d\n", sizeof(char), sizeof(int), sizeof(struct st));
  return 0;
}





