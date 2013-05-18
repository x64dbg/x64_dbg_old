#ifndef _COMMAND_H
#define _COMMAND_H

#include "_global.h"

//functions
bool cmdcontains(const char* cmd, const char* cmd_list);
int cmdnum(const char* cmd, const char** command_list, unsigned int command_list_size);
void commandloop(CBCOMMAND cbCommand);

#endif // _COMMAND_H
