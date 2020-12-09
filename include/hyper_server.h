#ifndef _HYPER_SERVER_H
#define _HYPER_SERVER_H

#define MAX_INPUT_BUFFER 1024

#include "commands.h"

#include <hyper.h>
#include <stdio.h>

void usage(void);

char** 
GetArgs(
    char                *a_str, 
    char                a_delim, 
    size_t              *count
);

extern int isConnected;

#endif
