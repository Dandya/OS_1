#include <stdio.h>

int main(int argc, char** args)
{
    char symbol;
    if(argc == 1)
    {
        printf("Input one symbol: ");
        symbol = getchar();
    }
    else
    {
        symbol = args[1][0];
    }
    FILE* boothw = fopen("./floppy.img", "r+");
    if(boothw == NULL)
    {
        fprintf(stderr,"Error: don't open file \"floppy.img\"\n");
        return 1;
    }
    char byte = '\0';
    while(byte != 'D')
    {
        if((byte = fgetc(boothw)) == EOF)
        {
            fprintf(stderr, "Error: don't read byte from \"floppy.img\"\n");
            return 2;
        }
        if(byte == 'j') // this "push"
        {
            continue;
        }
        else
        {
            fseek(boothw, -1, SEEK_CUR);
            byte = byte ^ symbol;
            if(fwrite(&byte, 1, 1, boothw) != 1)
            {
                fprintf(stderr, "Error: don't write byte to \"floppy.img\"\n");
                return 3;
            }
        }
        byte ^= symbol;
    }
    fclose(boothw);
    return 0;
}
