#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/**
 * getSizeInputFile
 * * Function returns size of file @input
 */
uint64_t getSizeInputFile(FILE* input)
{
    uint64_t size;
    fseek(input, 0, SEEK_END);
    size = ftell(input);
    fseek(input, 0, SEEK_SET);
    return size;
}
/**
 * sendFile
 * * Function sends messages with data from file 
 * ! Function doesn't responsible for @file != NULL
 */
void sendFile(int cd, int sizeFile, FILE* file)
{
    int msgLen = 0;
    char buf[8];
    while(sizeFile) // sizeFile != 0
    {
        msgLen = fread(buf, 1, 8, file);
        msgLen = send(cd, buf, msgLen, 0);
        if(msgLen <= 0)
        {
            fprintf(stderr, "Error sending message\n");
            exit(6);
        }
        sizeFile -= msgLen;
    }
}
/**
 * sendName
 * * Function sends messages with name 
 * ! Function doesn't responsible for @name != NULL
 */
void sendName(int cd, int sizeName, char* name)
{
    int msgLen = 0;
    while(sizeName) // sizeName != 0
    {
        msgLen = send(cd, name, 8, 0);
        if(msgLen <= 0)
        {
            fprintf(stderr, "Error sending message\n");
            exit(6);
        }
        name += msgLen;
        sizeName -= msgLen;
    }
}
//----------------------------------------------------------------
int main(int argc, char* argv[])
{
    // declaration client descriptor
    int cd;
    
    // declaration of struct for create address
    struct sockaddr_in addr;
    // create address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(addr.sin_addr.s_addr == INADDR_NONE) 
    {
        fprintf(stderr, "Error working inet_addr\n");
        return 2;
    }
    // create new socket and return file descriptor
    cd = socket(AF_INET, SOCK_STREAM, 0);
    if(cd < 0) 
    {
        fprintf(stderr, "Error calling socket\n");
        return 1;
    }   
    //connect to server
    if(connect(cd, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
    {
        fprintf(stderr, "Error connection to server\n");
        return 3;
    }
    char fileName[] = "../../../../../../../../proc/fi1";
    FILE* file = fopen("file.test", "r");
    if(file == NULL)
    {
        fprintf(stderr, "Error opening test file\n");
        close(cd);
        return 5;
    }
    int msg[] = {strlen(fileName)+1, getSizeInputFile(file) };
    int msgLen;
    // send sizes
    msgLen = send(cd, msg, sizeof(msg), 0);
    if(msgLen != sizeof(msg))
    {
        fprintf(stderr, "Error sending\n");
        close(cd);
        return 4;
    }
    // send name of file
    sendName(cd,strlen(fileName)+1, fileName);
    // send file
    sendFile(cd, getSizeInputFile(file), file);
    fclose(file);
    close(cd);
    return 0;

}
