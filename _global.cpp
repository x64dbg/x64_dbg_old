#include "_global.h"

bool scmp(const char* a, const char* b)
{
    dbg("scmp");
    if(strcmp(a, b))
        return false;
    return true;
}

void dbg(const char* a)
{
    char b[256]="";
    strcpy(b,a);
}

void formathex(char* string)
{
    int len=strlen(string);
    _strupr(string);
    char* new_string=(char*)malloc(len+1);
    memset(new_string, 0, len+1);
    for(int i=0,j=0; i<len; i++)
        if(isxdigit(string[i]))
            j+=sprintf(new_string+j, "%c", string[i]);
    strcpy(string, new_string);
    free(new_string);
}

void formatdec(char* string)
{
    int len=strlen(string);
    _strupr(string);
    char* new_string=(char*)malloc(len+1);
    memset(new_string, 0, len+1);
    for(int i=0,j=0; i<len; i++)
        if(isdigit(string[i]))
            j+=sprintf(new_string+j, "%c", string[i]);
    strcpy(string, new_string);
    free(new_string);
}
