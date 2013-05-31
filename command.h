#ifndef _COMMAND_H
#define _COMMAND_H

#include "_global.h"

//typedefs
typedef bool (*CBCOMMAND)(const char*);

struct COMMAND
{
    char* name;
    CBCOMMAND cbCommand;
    bool debugonly;
    COMMAND* next;
};

//functions
COMMAND* cmdinit();
bool cmdnew(COMMAND* command_list, const char* name, CBCOMMAND cbCommand, bool debugonly);
COMMAND* cmdget(COMMAND* command_list, const char* cmd);
CBCOMMAND cmdset(COMMAND* command_list, const char* name, CBCOMMAND cbCommand, bool debugonly);
bool cmddel(COMMAND* command_list, const char* name);
void cmdloop(COMMAND* command_list, CBCOMMAND cbUnknownCommand);

#endif // _COMMAND_H
