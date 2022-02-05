#include <stdio.h>
#include <mcheck.h>
#include <malloc.h>
#include <stdlib.h>
//----------------------------------------------------------------
#ifndef SMARTMEM_H
#define SMARTMEM_H
// @indexAllocatedMemory - counter for created massive @allocatedMemory
static long long sizeAllocatedMemory = 0; 
static void** allocatedMemory = NULL;
static long long indexInAllocatedMemory = 0;
/**
 * new
 * * Function return new memory size of @size
 * ! Function is not responsible for result of malloc
 */
void* new(size_t size)
{
    sizeAllocatedMemory++;
    return malloc(size);
}
/** 
 * constructor
 * * Function prepares to work with mtrace 
 */
static void __attribute__((constructor)) constructor(void)
{
    FILE* mem_log = fopen("mem.log", "w");
    if(mem_log == NULL)
    {
        fprintf(stderr, "ERROR: Невозможно создать фаил mem.log\nВыделенная память не будет самостоятельно очищена!");
    }
    fclose(mem_log);
    if(setenv("MALLOC_TRACE", "mem.log", 1))
    {
        fprintf(stderr, "ERROR: Невозможно создать переменную окружения MALLOC_TRACE!\nВыделенная память не будет самостоятельно очищена!");
    }
    mtrace();
}
/** 
 * readMem
 * * Function reads pointer value and returns this pointer 
 */
static void* readMem(FILE* mem_log)
{
    // @ptr - pointer to the memory
    void* ptr; 
    // @ptrHex - pointer value in hex
    char ptrHex[19]; 
    // read 19 bytes. 
    // Because pointer value in hex have mask: "0x****************" - is 19 bytes 
    if(fgets(ptrHex, 19, mem_log) == NULL)
    {
        // EOF
        return NULL;
    }
    if(ptrHex[1] == 'x')
    {
        ptr = (void*)strtoll(ptrHex, NULL, 16); // convert from string hex to long long int
    }
    else
    {
        return NULL;
    }
    return ptr;
}
/** 
 * addPtr
 * * Function adds pointer in massive @allocatedMemory
 */
static void addPtr(void* ptr)
{
    allocatedMemory[indexInAllocatedMemory] = ptr;
    indexInAllocatedMemory++;
    if(indexInAllocatedMemory >= sizeAllocatedMemory) // if user used simple malloc in own program
    {
        allocatedMemory = (void**)realloc(allocatedMemory, 2*sizeAllocatedMemory*sizeof(void**));
        sizeAllocatedMemory = 2*sizeAllocatedMemory;
    }
}
/** 
 * deletePtr
 * * Function deletes pointer in massive @allocatedMemory
 */
static void deletePtr(void* ptr)
{
    for(int index = 0; index <= indexInAllocatedMemory; index++)
    {
        if(allocatedMemory[index] == ptr)
        {
            allocatedMemory[index] = NULL;
            return;
        }
    }
}
/**
 * freeAll
 * * Function frees all allocated memory which didn't free
 */
static void freeAll()
{
#ifdef DEBUG
    long long countFreePtrs = 0;
#endif
    for(long long index = 0; index < indexInAllocatedMemory; index++)
    {
        if(allocatedMemory[index] != NULL)
        {
#ifdef DEBUG
            printf("free ptr = %p - index %lld\n", allocatedMemory[index], index);
            countFreePtrs++;
#endif
            free(allocatedMemory[index]);
        }
    }
#ifdef DEBUG
    printf("free ptrs = %lld\n", countFreePtrs);
#endif
}
/**
 * readActionWithMem
 * * Function reads the action with memory and does this action
 */
static void readActionWithMem(FILE* stream)
{
    char symbol = fgetc(stream);
    while (symbol != EOF)
    {
        symbol = fgetc(stream);
        if(symbol == ']')
        {
            fseek(stream, 1, SEEK_CUR); // is action
            symbol = fgetc(stream);
            if(symbol == '+')
            {
                fseek(stream, 1, SEEK_CUR); // is 0
                addPtr(readMem(stream));
            }
            else // is '-'
            {
                fseek(stream, 1, SEEK_CUR); // is 0
                deletePtr(readMem(stream));
            }
        }
    }
}
/** 
 * freeMem 
 */
static void __attribute__((destructor)) freeMem(void)
{
    muntrace(); // poweroff work functhion mtrace, then clearing memory will not be written to mem.log. Use DEBUG
    FILE* mem_log = fopen("./mem.log", "r");
    if(mem_log == NULL)
    {
        fprintf(stderr, "ERROR: Невозможно открыть mem.log\nВыделенная память не будет самостоятельно очищена!");
        exit(1);
    }
    sizeAllocatedMemory++;
    allocatedMemory = (void**)malloc(sizeAllocatedMemory*sizeof(void**));
    readActionWithMem(mem_log);
    freeAll();
    free(allocatedMemory);
#ifndef DEBUG
    if(system("rm mem.log"))
    {
        fprintf(stderr, "ERROR: Невозможно удалить mem.log\n");
    }
#endif
}
#endif
