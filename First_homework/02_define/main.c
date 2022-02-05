#include <stdio.h>
#include <stdlib.h>

#ifndef MY_NAME
#define MY_NAME "World"
#endif

int main(int argc, char* argv[])
{
    printf("Hello, %s!\n", MY_NAME);
    return 0;
}
