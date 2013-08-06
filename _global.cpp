#include "_global.h"

void* emalloc(size_t size)
{
    unsigned char* a=new unsigned char[size];
    if(!a)
    {
        MessageBoxA(0, "Could not allocate memory", "Error", MB_ICONERROR);
        ExitProcess(1);
    }
    return a;
}

void efree(void* ptr)
{
    delete[] (unsigned char*)ptr;
}

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
    for(int i=strlen(temp); i<len; i++)
    {
        if(!temp[i])
        {
            if(!strcasecmp(temp+i+1, cmd))
                return true;
            i+=strlen(temp+i+1);
        }
    }
    return false;
}

bool scmp(const char* a, const char* b)
{
    if(strcmp(a, b))
        return false;
    return true;
}

void formathex(char* string)
{
    int len=strlen(string);
    _strupr(string);
    char* new_string=(char*)emalloc(len+1);
    memset(new_string, 0, len+1);
    for(int i=0,j=0; i<len; i++)
        if(isxdigit(string[i]))
            j+=sprintf(new_string+j, "%c", string[i]);
    strcpy(string, new_string);
    efree(new_string);
}

void formatdec(char* string)
{
    int len=strlen(string);
    _strupr(string);
    char* new_string=(char*)emalloc(len+1);
    memset(new_string, 0, len+1);
    for(int i=0,j=0; i<len; i++)
        if(isdigit(string[i]))
            j+=sprintf(new_string+j, "%c", string[i]);
    strcpy(string, new_string);
    efree(new_string);
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
