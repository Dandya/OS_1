#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
//----------------------------------------------------------------
/**
 * createScript
 * * Function create bash script that search files on mask *.so 
 */
int createScript()
{
    char script[] = "#!/bin/bash\nNAME_FILE=\"$(pwd)/libs.txt\"\nfunction search\n{\n    for dir in $(ls -d */)\n    do\n        echo $dir\n        cd $dir\n        search $dir\n     done\n    echo \"$(find  $(pwd)/*.so)\" >> $NAME_FILE\n    cd ../\n}\nolsIFS=$IFS\nIFS=$'\\n'\nsearch ./\nIFS=$oldIFS\nreturn 0"; 
    FILE* bashScript = fopen("search.sh", "w");
    if(bashScript == NULL)
    {
        fprintf(stderr, "Ошибка создания bash скрипта: Невозможно создать фаил\n");
        exit(1);
    }
    if(fputs(script, bashScript) == EOF)
    {
        fprintf(stderr, "Ошибка создания bash скрипта: Невозможно записать данные в фаил\n");
        exit(1);
    }
    fclose(bashScript);
    
}
/**
 * readLine
 * * Function reads one line from @file and return count of reading chars without '\0'
 * ! function is not responsible for line != NULL and file != NULL
 */
int readLine(char* line, FILE* file)
{
    int index = 0;
    line[index] = fgetc(file);
    while(line[index] != '\n' && line[index] != EOF)
    {
        index++;
        line[index] = fgetc(file);
    }
    line[index] = '\0';
    return index;
}
//----------------------------------------------------------------
int main(int argc, char** argv)
{
    int result = system("bash ./search.sh > /dev/null 2> /dev/null");
    FILE* libs = fopen("libs.txt", "r");
    if(libs == NULL)
    {
        createScript();
        result = system("bash ./search.sh > /dev/null 2> /dev/null");
        libs = fopen("libs.txt", "r");
    }
    if(libs == NULL)
    {
        fprintf(stderr, "Ошибка поиска библиотек: не создался или не открывается фаил libs.txt\nДля решения попробуйте перезапустить программу\n");
        exit(3);
    }
    //read libs.txt and open *.so
    void* handle;
    char* (*pushkin)(char* text);
    char* nLines = NULL;
    char* nameLib = (char*)malloc(257);
    int readableChars = readLine(nameLib, libs);
    while(readableChars > 1) 
    {
        handle = dlopen(nameLib, RTLD_LAZY);
        if(handle == NULL)
        {
            fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
            readableChars = readLine(nameLib, libs);
            continue;
        }
        pushkin = (char* (*)(char*)) dlsym(handle, "pushkin");
        if(pushkin != NULL)
        {
            nLines = pushkin(nLines);
        }
        else 
        {
            fprintf(stderr, "Ошибка получения функции: %s\n", dlerror());
        }
        dlclose(handle);
        readableChars = readLine(nameLib, libs);
    }
    fclose(libs);
    if(nLines != NULL)
    {
        printf("%s", nLines);
    }
    system("rm ./libs.txt");
    return 0;
}
