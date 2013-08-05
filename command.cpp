#include "command.h"
#include "argument.h"
#include "console.h"
#include "debugger.h"
#include "math.h"

static COMMAND* cmdfind(COMMAND* command_list, const char* name, COMMAND** link)
{
    COMMAND* cur=command_list;
    if(!cur->name)
        return 0;
    COMMAND* prev=0;
    while(cur)
    {
        if(arraycontains(cur->name, name))
        {
            if(link)
                *link=prev;
            return cur;
        }
        prev=cur;
        cur=cur->next;
    }
    return 0;
}

COMMAND* cmdinit()
{
    COMMAND* cmd=(COMMAND*)malloc(sizeof(COMMAND));
    memset(cmd, 0, sizeof(COMMAND));
    return cmd;
}

bool cmdnew(COMMAND* command_list, const char* name, CBCOMMAND cbCommand, bool debugonly)
{
    if(!command_list or !cbCommand or !name or !*name or cmdfind(command_list, name, 0))
        return false;
    COMMAND* cmd;
    bool nonext=false;
    if(!command_list->name)
    {
        cmd=command_list;
        nonext=true;
    }
    else
        cmd=(COMMAND*)malloc(sizeof(COMMAND));
    memset(cmd, 0, sizeof(COMMAND));
    cmd->name=(char*)malloc(strlen(name)+1);
    strcpy(cmd->name, name);
    cmd->cbCommand=cbCommand;
    cmd->debugonly=debugonly;
    COMMAND* cur=command_list;
    if(!nonext)
    {
        while(cur->next)
            cur=cur->next;
        cur->next=cmd;
    }
    return true;
}

COMMAND* cmdget(COMMAND* command_list, const char* cmd)
{
    char new_cmd[deflen]="";
    strcpy(new_cmd, cmd);
    int len=strlen(new_cmd);
    int start=0;
    while(new_cmd[start]!=' ' and start<len)
        start++;
    new_cmd[start]=0;
    COMMAND* found=cmdfind(command_list, new_cmd, 0);
    if(!found)
        return 0;
    return found;
}

CBCOMMAND cmdset(COMMAND* command_list, const char* name, CBCOMMAND cbCommand, bool debugonly)
{
    if(!cbCommand)
        return 0;
    COMMAND* found=cmdfind(command_list, name, 0);
    if(!found)
        return 0;
    CBCOMMAND old=found->cbCommand;
    found->cbCommand=cbCommand;
    found->debugonly=debugonly;
    return old;
}

bool cmddel(COMMAND* command_list, const char* name)
{
    COMMAND* prev=0;
    COMMAND* found=cmdfind(command_list, name, &prev);
    if(!found)
        return false;
    free(found->name);
    if(found==command_list)
    {
        COMMAND* next=command_list->next;
        if(next)
        {
            memcpy(command_list, command_list->next, sizeof(COMMAND));
            command_list->next=next->next;
            free(next);
        }
        else
            memset(command_list, 0, sizeof(COMMAND));
    }
    else
    {
        prev->next=found->next;
        free(found);
    }
    return true;
}

static void specialformat(char* string)
{
    int len=strlen(string);
    char* found=strstr(string, "=");
    char* str=(char*)malloc(len*2);
        memset(str, 0, len*2);
    if(found) //contains =
    {
        char* a=(found-1);
        *found=0;
        found++;
        if(mathisoperator(*a)>2) //x*=3 -> x=x*3
        {
            char op=*a;
            *a=0;
            sprintf(str, "mov %s,%s%c%s", string, string, op, found);
        }
        else
            sprintf(str, "mov %s,%s", string, found);
        strcpy(string, str);
    }
    else if((string[len-1]=='+' and string[len-2]=='+') or (string[len-1]=='-' and string[len-2]=='-')) //eax++/eax--
    {
        string[len-2]=0;
        char op=string[len-1];
        sprintf(str, "mov %s,%s%c1", string, string, op);
        strcpy(string, str);
    }
    free(str);
}

void cmdloop(COMMAND* command_list, CBCOMMAND cbUnknownCommand)
{
    char command[deflen]="";
    bool bLoop=true;
    while(bLoop)
    {
        printf(">");
        consolesetlasty();
        fgets(command, deflen, stdin);
        command[strlen(command)-1]=0;
        int len=strlen(command);
        if(len)
        {
            argformat(command);
            COMMAND* cmd=cmdget(command_list, command);
            if(!cmd)
            {
                specialformat(command);
                cmd=cmdget(command_list, command);
            }
            if(!cmd)
            {
                mathformat(command);
                bLoop=cbUnknownCommand(command);
            }
            else
            {
                CBCOMMAND cbCommand=cmd->cbCommand;
                if(cmd->debugonly and !IsFileBeingDebugged())
                    cputs("this command is debug-only");
                else
                {
                    if(!cbCommand)
                    {
                        mathformat(command);
                        bLoop=cbUnknownCommand(command);
                    }
                    else
                        bLoop=cbCommand(command);
                }
            }
        }
    }
}
