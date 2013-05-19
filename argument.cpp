#include "argument.h"

void formatarg(char* cmd)
{
    //puts("[DEBUG] formatarg");
    int len=strlen(cmd);
    if(!len)
        return;
    int start=0;
    int minstart=0;
    while(cmd[start]!=' ' and cmd[start]!=',' and start<len)
        start++;
    start++;
    while(cmd[start]==' ') //remove prepended spaces
    {
        cmd[start]=0;
        start++;
        minstart++;
    }
    if(start==len)
    {
        len=strlen(cmd);
        if(cmd[len-1]==' ')
            cmd[len-1]=0;
        printf("\"%s\"\n", cmd);
        _strlwr(cmd);
        return;
    }
    while(cmd[start]==',') //remove prepended commas
        start++;

    int quote_count=0;
    for(int i=start; i<len; i++)
        if(cmd[i]=='"' and cmd[i-1]!='\\')
            quote_count++;

    char temp0[deflen]="";
    bool has_strings=false;
    if(quote_count and !(quote_count%2))
        has_strings=true;
    int string_count=0;
    if(has_strings)
    {
        string_count=quote_count/2;
        for(int i=start,j=0; i<len; i++)
        {
            if(cmd[i]=='"' and cmd[i-1]!='\\')
            {
                //TODO: fix strings
            }
            else
                j+=sprintf(temp0+j, "%c", cmd[i]);
        }
    }

    char temp1[deflen]="";
    for(int i=start,j=0; i<len; i++) //remove non-escaped spaces
        if(!(cmd[i]==' ' and cmd[i-1]!='\\'))
            j+=sprintf(temp1+j, "%c", cmd[i]);
    len=strlen(temp1);
    while(temp1[len-1]==',' and temp1[len-2]!='\\') //remove appended (non-escaped) commas
        len--;
    temp1[len]=0;
    //printf("[DEBUG] temp1, \"%s\"[%d]\n", temp1, strlen(temp1));
    char temp2[deflen]="";
    len=strlen(temp1);
    for(int i=0,j=0; i<len; i++)
    {
        while(temp1[i]==',' and temp1[i-1]!='\\' and temp1[i+1]==',') //remove double (non-prefixed) commas
            i++;
        j+=sprintf(temp2+j, "%c", temp1[i]);
    }
    //printf("[DEBUG] temp2: \"%s\"[%d]\n", temp2, strlen(temp2));
    _strlwr(cmd);
    if(strlen(temp2))
    {
        cmd[start-1]=' ';
        strcpy(cmd+start-minstart, temp2);
    }
    //printf("[DEBUG] cmd: \"%s\"[%d]\n", cmd, strlen(cmd));
}

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
    for(int i=start; i<len; i++)
    {
        if(cmd[i]==',' and cmd[i-1]!='\\')
            arg_count++;
    }
    //printf("[DEBUG] arg_count: %d\n", arg_count);
    return arg_count;
}

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
    {
        if(temp[i]==',' and temp[i-1]!='\\')
            temp[i]=0;
    }
    char new_temp[deflen]="";
    int new_len=len;
    for(int i=0,j=0; i<len; i++) //handle escape characters
    {
        if(temp[i]=='\\' and (temp[i+1]==',' or temp[i+1]==' '))
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
