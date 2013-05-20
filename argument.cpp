#include "argument.h"

/*
formatarg:
01) get command (first space) and lowercase
02) get arguments
03) remove double quotes (from arguments)
04) temp. remove double backslash
05) remove prepended/appended non-escaped commas and spaces (from arguments)
    a) prepended
    b) appended
06) get quote count, ignore escaped (from arguments)
07) process quotes (from arguments):
    a) zero quotes
    b) restore double backslash
    c) escape commas and spaces
08) temp. remove double backslash
09) remove unescaped double commas (from arguments)
10) remove unescaped spaces (from arguments)
11) restore double backslash
12) combine formatted arguments and command
*/
void formatarg(char* cmd)
{
    //puts("[DEBUG] formatarg");
    char command[deflen]="";
    strcpy(command, cmd);
    int len=strlen(command);
    int start=0;
    for(int i=0; i<len; i++)
        if(command[i]==' ')
        {
            command[i]=0;
            start=i+1;
            break;
        }
    if(!start)
        start=len;
    _strlwr(command);
    //printf("[DEBUG] 1:command: \"%s\"[%d]\n", command, strlen(command));
    char arguments_[deflen]="";
    char* arguments=arguments_;
    strcpy(arguments, command+start);
    //printf("[DEBUG] 2:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    char temp[deflen]="";
    len=strlen(arguments);
    for(int i=0,j=0; i<len; i++)
    {
        if(arguments[i]=='"' and arguments[i+1]=='"')
            i+=2;
        j+=sprintf(temp+j, "%c", arguments[i]);
    }
    strcpy(arguments, temp);
    //printf("[DEBUG] 3:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    len=strlen(arguments);
    for(int i=0; i<len; i++)
        if(arguments[i]=='\\' and arguments[i+1]=='\\')
        {
            arguments[i]=1;
            arguments[i+1]=1;
        }
    //printf("[DEBUG] 4:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    while((*arguments==',' or *arguments==' ') and *(arguments-1)!='\\')
        arguments++;
    //printf("[DEBUG] 5a:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    len=strlen(arguments);
    while((arguments[len-1]==' ' or arguments[len-1]==',') and arguments[len-2]!='\\')
        len--;
    arguments[len]=0;
    //printf("[DEBUG] 5b:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    len=strlen(arguments);
    int quote_count=0;
    for(int i=0; i<len; i++)
        if(arguments[i]=='"')
            quote_count++;
    //printf("[DEBUG] 6:quote_count: %d\n", quote_count);
    if(!(quote_count%2))
    {
        for(int i=0; i<len; i++)
            if(arguments[i]=='"')
                arguments[i]=0;
        //dbg("[DEBUG] 7a:arguments");
        for(int i=0; i<len; i++)
            if(arguments[i]==1 and arguments[i+1]==1)
            {
                arguments[i]='\\';
                arguments[i+1]='\\';
            }
        //dbg("[DEBUG] 7b:arguments");
        for(int i=0,j=0; i<len; i++)
        {
            if(!arguments[i])
            {
                i++;
                int len2=strlen(arguments+i);
                for(int k=0; k<len2; k++)
                {
                    if(arguments[i+k]==',' or arguments[i+k]==' ' or arguments[i+k]=='\\')
                        j+=sprintf(temp+j, "\\%c", arguments[i+k]);
                    else
                        j+=sprintf(temp+j, "%c", arguments[i+k]);
                }
                i+=len2;
            }
            else
                j+=sprintf(temp+j, "%c", arguments[i]);
        }
        arguments=arguments_;
        strcpy(arguments, temp);
    }
    //printf("[DEBUG] 7c:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    len=strlen(arguments);
    for(int i=0; i<len; i++)
        if(arguments[i]=='\\' and arguments[i+1]=='\\')
        {
            arguments[i]=1;
            arguments[i+1]=1;
        }
    //printf("[DEBUG] 8:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    len=strlen(arguments);
    for(int i=0,j=0; i<len; i++)
    {
        if(arguments[i]==',' and arguments[i+1]==',')
            i+=2;
        j+=sprintf(temp+j, "%c", arguments[i]);
    }
    strcpy(arguments, temp);
    //printf("[DEBUG] 9:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    len=strlen(arguments);
    for(int i=0,j=0; i<len; i++)
    {
        while(arguments[i]==' ' and arguments[i-1]!='\\')
            i++;
        j+=sprintf(temp+j, "%c", arguments[i]);
    }
    strcpy(arguments, temp);
    //printf("[DEBUG] 10:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    len=strlen(arguments);
    for(int i=0; i<len; i++)
        if(arguments[i]==1 and arguments[i+1]==1)
        {
            arguments[i]='\\';
            arguments[i+1]='\\';
        }
    //printf("[DEBUG] 11:arguments: \"%s\"[%d]\n", arguments, strlen(arguments));
    if(strlen(arguments))
        sprintf(cmd, "%s %s", command, arguments);
    else
        strcpy(cmd, command);
    //printf("[DEBUG] cmd: \"%s\"[%d]\n", cmd, strlen(cmd));
}

/*
1) remove double backslash
2) count unescaped commas
*/
int getargcount(const char* cmd)
{
    //puts("[DEBUG] getargcount");
    int len=strlen(cmd);
    if(!len)
        return -1;

    int arg_count=0;

    int start=0;
    while(cmd[start]!=' ' and start<len)
        start++;
    if(start==len)
    {
        //printf("[DEBUG] arg_count: %d\n", arg_count);
        return arg_count;
    }
    arg_count=1;
    char temp[deflen]="";
    strcpy(temp, cmd);
    for(int i=start; i<len; i++)
        if(temp[i]=='\\' and temp[i+1]=='\\')
        {
            temp[i]=1;
            temp[i+1]=1;
        }
    for(int i=start; i<len; i++)
    {
        if(temp[i]==',' and temp[i-1]!='\\')
            arg_count++;
    }
    //printf("[DEBUG] arg_count: %d\n", arg_count);
    return arg_count;
}
/*
1) get arg count
2) remove double backslash
3) zero non-escaped commas
4) restore double backslash
5) handle escape characters
*/
bool getarg(const char* cmd, char* arg, int arg_num, bool optional)
{
    int argcount=getargcount(cmd);
    if((arg_num+1)>argcount)
    {
        if(!optional)
            printf("missing argument nr %d\n", arg_num+1);
        return false;
    }
    int len=strlen(cmd);
    int start=0;
    while(cmd[start]!=' ')
        start++;
    while(cmd[start]==' ')
        start++;
    char temp[deflen]="";
    strcpy(temp, cmd+start);
    len=strlen(temp);
    for(int i=0; i<len; i++)
        if(temp[i]=='\\' and temp[i+1]=='\\')
        {
            temp[i]=1;
            temp[i+1]=1;
        }
    for(int i=0; i<len; i++)
    {
        if(temp[i]==',' and temp[i-1]!='\\')
            temp[i]=0;
    }
    for(int i=0; i<len; i++)
        if(temp[i]=='\\' and temp[i+1]=='\\')
        {
            temp[i]='\\';
            temp[i+1]='\\';
        }
    char new_temp[deflen]="";
    int new_len=len;
    for(int i=0,j=0; i<len; i++) //handle escape characters
    {
        if(temp[i]=='\\' and (temp[i+1]==',' or temp[i+1]==' ' or temp[i+1]=='\\'))
        {
            new_len--;
            j+=sprintf(new_temp+j, "%c", temp[i+1]);
            i++;
        }
        else
            j+=sprintf(new_temp+j, "%c", temp[i]);
    }
    len=new_len;
    memcpy(temp, new_temp, len+1);
    if(arg_num==0) //first argument
    {
        strcpy(arg, temp);
        return true;
    }
    for(int i=0,j=0; i<len; i++)
    {
        if(!temp[i])
            j++;
        if(j==arg_num)
        {
            strcpy(arg, temp+i+1);
            return true;
        }
    }
    return false;
}
