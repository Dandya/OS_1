#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct st {
  char a;
  char crc;
  int b;
};

void my_init(struct st* st)
{
  char crc = 0;
  int i;
  st->a = 2;
  st->crc = 0;
  st->b = 5;
  for ( i = 0 ; i < sizeof(struct st) ; i++ ){
    crc ^= ((char*)st)[i];
  }
  st->crc = crc;
}

void my_check(struct st* st)
{
  char crc, orig;
  crc = 0;
  orig = st->crc;
  
  st->crc = 0;
  for ( int i = 0 ; i < sizeof(struct st) ; i++ ){
    crc ^= ((char*)st)[i];
  }
  if ( crc != orig ) {
    printf("ERROR\n");
    exit(0);
  }
  st->crc = orig;
}
