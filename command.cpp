#include "command.h"
#include "argument.h"

bool cmdcontains(const char* cmd_list, const char* cmd)
{
    dbg("cmdcontains");
    char temp[deflen]="";
    strcpy(temp, cmd_list);
    int len=strlen(cmd_list);
    for(int i=0; i<len; i++)
        if(temp[i]==1)
            temp[i]=0;
    if(scmp(temp, cmd))
        return true;
    for(int i=0; i<len; i++)
    {
        if(!temp[i])
            if(scmp(temp+i+1, cmd))
                return true;
    }
    return false;
}

int cmdnum(const char* cmd, const char** command_list, unsigned int command_list_size)
{
    char new_cmd[deflen]="";
    strcpy(new_cmd, cmd);
    int len=strlen(new_cmd);
    int start=0;
    while(new_cmd[start]!=' ' and start<len)
        start++;
    new_cmd[start]=0;

    if(!strlen(new_cmd))
        return -1;
    for(int i=0; i<(int)(command_list_size/sizeof(char*)); i++)
        if(cmdcontains(command_list[i], new_cmd))
            return i;
    return -1;
}

void commandloop(CBCOMMAND cbCommand)
{
    char command_[deflen]="";
    char* command=command_;
    bool bLoop=true;
    while(bLoop)
    {
        printf(">");
        fgets(command, deflen, stdin);
        command[strlen(command)-1]=0;
        formatarg(command);
        bLoop=cbCommand(command);
    }
}
