#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
/**
 * writeValue
 * * Function writes value from memSecond in memFirst
 * ! Function doesn't responsible for @memSecond != NULL, @memFirst != NULL
 */
void writeValue(char* memFirst, char* memSecond, int lengthSecond)
{
    for(int index = 0; index < lengthSecond; index++)
    {
        memFirst[index] = memSecond[index];
    }
}
/**
 * divFileNameAndPath
 * * Function divides @file by file name and path
 * ! Function doesn't responsible for @file != NULL
 * ! return pointer on new path
 */
char* divFileNameAndPath(char* file, int size, char* fileName)
{
#ifdef DEBUG
    printf("divFileNameAndPath\n");
#endif
    int index = size;
    while(file[index] != '/')
    {
        index--;
        if(index == -1)
        {
            break;
        }
    }
    char* path = (char*)malloc(index+4);// ! if index == -1
    writeValue(path, file, index+1);
    if(index == -1)
    {
        writeValue(path, "", 1);
    }
    else if(index == 0)
    {
        writeValue(path+1, "", 1);
    }
    else
    {
        writeValue(path+index, "", 1);
    }
    if(path[0] == '\0')
    {
        writeValue(path, "./", 3);
    }
    writeValue(fileName, file+index+1, size - index);
    return path;
}

/**
 * checkPath
 * * Function checks valid @pash
 * ! Function doesn't responsible for @path != NULL
 */
int checkPath(char* path)
{
    char validPath[] = "/home/server/";
    for(int i = 0; i < 13; i++)
    {
        if(path[i] != validPath[i])
        {
            return 1;
        }
    }
    return 0;
}
/**
 * createFile
 * * Function receives messages with data and creates the file
 */
void createFile(int cd, int sizeName, int sizeFile)
{
    int sizeNameS = sizeName;
    char* fileName = (char*)malloc(sizeName);
    long long* msg = (long long*)malloc(sizeof(long long));
    int msgLen = 0;
    int index = 0;
    // receive name of file
    while(sizeName) // while(sizeName != 0)
    {
#ifdef DEBUG
       // printf("sizeName = %d\n", sizeName);
#endif
        msgLen = recv(cd, msg, sizeName >= 8? 8: sizeName, MSG_WAITALL);
#ifdef DEBUG
        printf("%s\n", (char*)msg);
        printf("msglen = %d\n", msgLen);
#endif
        if(msgLen <= 0)
        {
            fprintf(stderr, "Error receiving message with file name\n");
            free(fileName);
            free(msg);
            return;
        }
        
        writeValue(fileName+index, (char*)msg, msgLen);
        index += msgLen;
        sizeName -= msgLen;
    }
    char* path = divFileNameAndPath(fileName, sizeNameS, fileName);
#ifdef DEBUG
        printf( "%s , realpath\n", path);
#endif
    struct stat check;
    index = stat(path, &check);
    if(index == -1) // dir doesn't exist
    {
        fprintf(stderr, "stat error\n");
        free(path);
        free(fileName);
        free(msg);
        return;
    }
    else if(S_ISDIR(check.st_mode))
    {
#ifdef DEBUG
    printf("It is path\n");
#endif       
    }
    else
    {
        fprintf(stderr, "stat error\n");
        free(path);
        free(fileName);
        free(msg);
        return;
    }
    char* absolutePath  = realpath(path, NULL);
    strcat(absolutePath, "/");
#ifdef DEBUG
    printf("absolute path = %s\n", absolutePath);
#endif
    if(absolutePath == NULL)
    {
        fprintf(stderr, "absolutePath == NULL\n");
        free(path);
        free(fileName);
        free(msg);
        return;
    }
    else if(checkPath(absolutePath))
    {
        fprintf(stderr, "invalid name\n");
        free(path);
        free(absolutePath);
        free(fileName);
        free(msg);
        return;
    }
    strcat(absolutePath, fileName);
#ifdef DEBUG
    printf("absolute name file = %s\n", absolutePath);
#endif
    // receives file
    FILE* file = fopen(absolutePath, "w");
    if(file == NULL)
    {
        fprintf(stderr, "Don't create file");
        free(path);
        free(absolutePath);
        free(fileName);
        free(msg);
        return;
    }
    while(sizeFile) // while(sizeFile != 0)
    {
#ifdef DEBUG
       // printf("sizeFile = %d\n", sizeFile);
#endif
        msgLen = recv(cd, msg, sizeFile >= 8? 8: sizeFile, MSG_WAITALL);
        if(msgLen <= 0)
        {
            fprintf(stderr, "Error receiving message in create file\n");
            return;
        }
        fwrite(msg, msgLen, 1, file);
        sizeFile -= msgLen;
    }
    free(path);
    free(absolutePath);
    free(fileName);
    free(msg);
    fclose(file);
}
//----------------------------------------------------------------
int main(int argc, char* argv[])
{
    // declaration server descriptor and client descriptor
    int sd, cd;
    // create new socket and return descriptor
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0) 
    {
	    fprintf(stderr, "Error calling socket\n");
	    return 1;
    }
    // declaration of struct for creating address
    struct sockaddr_in addr;
    // create address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    if(addr.sin_addr.s_addr == INADDR_NONE) 
    {
        fprintf(stderr, "Error working inet_addr\n");
        return 2;
    }
    // bind socket to address
    if(bind(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "Error binding socket with address\n");
        return 3;
    }
    // message
    // * halfMsg[0] = size of name file, halfMsg[1] = size of file
    int halfMsg[2];
    long long* msg = (long long*)halfMsg;
    int msgLen;
    // listen for connection
    if(listen(sd, 1) < 0) 
    {
        fprintf(stderr, "Error listening\n");
        return 4;
    }
    // need for ending accept
    struct pollfd fd [1];
    fd[0].fd = sd;
    // waiting connection
    fd[0].events = POLLIN;
    // begin
    while(1)
    {   
        int result = poll( fd, 1, 10000 ); // waiting for 10 sec
        if ( result == -1 )
        {
            fprintf(stderr, "Error poll\n");
        }
        else if(result == 0)
        {
            printf("It is all\n");
            close(sd);
            return 0;
        }
        else
        {
            // clean returned events 
            if (fd[0].revents == POLLIN)
            {
                fd[0].revents = 0;
            }
        }
        // accept connection
        if((cd = accept(sd, NULL, NULL)) < 0)
        {
            fprintf(stderr, "Error accepting connection\n");
            return 5;
        }
        // receiving message
        msgLen = recv(cd, msg, 8, MSG_WAITALL);
        if(msgLen <= 0)
        {
            fprintf(stderr, "Error receiving message\n");
            close(cd);
            continue;
        }
#ifdef DEBUG
        printf("nameLen = %d\nfileLen = %d\n", halfMsg[0], halfMsg[1]);
#endif
        if(halfMsg[0] <= 0 || halfMsg[1] < 0)
        {
            fprintf(stderr, "Error size of name file or file\n");
            return 6;
        }
        createFile(cd ,halfMsg[0], halfMsg[1]);
        close(cd);
    }
    // end
    close(sd);
    return 0;
}
