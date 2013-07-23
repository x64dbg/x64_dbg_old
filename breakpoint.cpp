#include "breakpoint.h"
#include "debugger.h"

BREAKPOINT* bpinit()
{
    BREAKPOINT* bp=(BREAKPOINT*)malloc(sizeof(BREAKPOINT));
    memset(bp, 0, sizeof(BREAKPOINT));
    return bp;
}

BREAKPOINT* bpfind(BREAKPOINT* breakpoint_list, const char* name, uint addr, BREAKPOINT** link)
{
    BREAKPOINT* cur=breakpoint_list;
    if(!cur or !cur->addr)
        return 0;
    BREAKPOINT* prev=0;
    while(cur)
    {
        if((name and arraycontains(cur->name, name)) or cur->addr==addr)
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

bool bpnew(BREAKPOINT* breakpoint_list, const char* name, uint addr, short oldbytes, BP_TYPE type)
{
    if(!breakpoint_list or !addr or bpfind(breakpoint_list, name, addr, 0))
        return false;
    BREAKPOINT* bp;
    bool nonext=false;
    if(!breakpoint_list->addr)
    {
        bp=breakpoint_list;
        nonext=true;
    }
    else
        bp=(BREAKPOINT*)malloc(sizeof(BREAKPOINT));
    memset(bp, 0, sizeof(BREAKPOINT));
    if(name and *name)
    {
        bp->name=(char*)malloc(strlen(name)+1);
        strcpy(bp->name, name);
    }
    bp->addr=addr;
    bp->type=type;
    bp->oldbytes=oldbytes;
    bp->enabled=true;
    BREAKPOINT* cur=breakpoint_list;
    if(!nonext)
    {
        while(cur->next)
            cur=cur->next;
        cur->next=bp;
    }
    return true;
}

bool bpsetname(BREAKPOINT* breakpoint_list, uint addr, const char* name)
{
    if(!name or !*name or !addr or bpfind(breakpoint_list, name, 0, 0))
        return false;
    BREAKPOINT* found=bpfind(breakpoint_list, 0, addr, 0);
    if(!found)
        return false;
    free(found->name);
    found->name=(char*)malloc(strlen(name)+1);
    strcpy(found->name, name);
    return true;
}

bool bpdel(BREAKPOINT* breakpoint_list, const char* name, uint addr)
{
    BREAKPOINT* prev=0;
    BREAKPOINT* found=bpfind(breakpoint_list, name, addr, &prev);
    if(!found)
        return false;
    if(found->name)
        free(found->name);
    if(found==breakpoint_list)
    {
        BREAKPOINT* next=breakpoint_list->next;
        if(next)
        {
            memcpy(breakpoint_list, breakpoint_list->next, sizeof(BREAKPOINT));
            breakpoint_list->next=next->next;
            free(next);
        }
        else
            memset(breakpoint_list, 0, sizeof(BREAKPOINT));
    }
    else
    {
        prev->next=found->next;
        free(found);
    }
    return true;
}
