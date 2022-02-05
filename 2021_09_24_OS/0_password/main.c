#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if ( argc == 1 ) {
    fprintf(stderr,"Usage: %s <password>\n", argv[0]);
    return 0;
  }

  if ( 0 == strcmp(argv[1],"password") ) {
    printf("Hello, USER\n");
  } else {
    printf("Bad 'password'\n");
  }
  return 0;
}
