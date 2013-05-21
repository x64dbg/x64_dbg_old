#include "command.h"
#include "argument.h"

int cmdnum(const char* cmd, COMMAND_LIST* command_list)
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
    int size=command_list->size/sizeof(char*);
    for(int i=0; i<size; i++)
        if(arraycontains(command_list->commands[i], new_cmd))
            return i+1;
    return 0;
}

void cmdloop(CBCOMMANDFATHER cbCommandFather, COMMAND_LIST* command_list)
{
    char command_[deflen]="";
    char* command=command_;
    bool bLoop=true;
    while(bLoop)
    {
        printf(">");
        fgets(command, deflen, stdin);
        command[strlen(command)-1]=0;
        argformat(command);
        bLoop=cbCommandFather(command, command_list);
    }
}

bool cmdadd(const char* cmd, CBCOMMANDCHILD cbCommand, COMMAND_LIST* command_list)
{
    return true;
}
