#include <stdio.h>

#ifndef MY_NAME
#define MY_NAME "World"
#endif

#ifdef DEBUG_MODE
#define DEBUG(msg) fprintf(stderr,"[%s:%d] %s: %s\n",        \
                           __FILE__,__LINE__,__func__, (msg));
#else
#define DEBUG(msg)
#endif

int function(char* name)
{
    DEBUG("Начало работы");
    printf("Hello, %s!\n", name);
    DEBUG("Конец работы");
}

int main(int argc, char* argv[])
{
    DEBUG("Начало работы");
    function(MY_NAME);
    DEBUG("Конец работы");
    return 0;
}
