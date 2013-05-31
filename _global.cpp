#include "_global.h"

bool arraycontains(const char* cmd_list, const char* cmd)
{
    if(!cmd_list or !cmd)
        return false;
    char temp[deflen]="";
    strcpy(temp, cmd_list);
    int len=strlen(cmd_list);
    for(int i=0; i<len; i++)
        if(temp[i]==1)
            temp[i]=0;
    if(!strcasecmp(temp, cmd))
        return true;
    for(int i=0; i<len; i++)
    {
        if(!temp[i])
        {
            i++;
            if(!strcasecmp(temp+i, cmd))
                return true;
            i+=strlen(temp+i);
        }
    }
    return false;
}

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

bool FileExists(const char* file)
{
    DWORD attrib=GetFileAttributes(file);
    return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool DirExists(const char* dir)
{
    DWORD attrib=GetFileAttributes(dir);
    return (attrib==FILE_ATTRIBUTE_DIRECTORY);
}
