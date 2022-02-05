#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
        if (sizeFile < 1000)
        {
            return; 
        }
        sizeFile -= msgLen;
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
    close(cd);
    return 0;

}