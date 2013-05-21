#ifndef _COMMAND_H
#define _COMMAND_H

#include "_global.h"

struct COMMAND_LIST
{
    unsigned int size;
    const char** commands;
};

//typdefs
typedef bool (*CBCOMMANDFATHER)(const char*, COMMAND_LIST*);
typedef bool (*CBCOMMANDCHILD)(const char*);

//functions
int cmdnum(const char* cmd, COMMAND_LIST* command_list);
void cmdloop(CBCOMMANDFATHER cbCommandFather, COMMAND_LIST* command_list);

#endif // _COMMAND_H
