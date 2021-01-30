#ifndef _HYPER_SERVER_H
#define _HYPER_SERVER_H

#define MAX_INPUT_BUFFER 1024

#include "commands.h"

#define HYPER_IMPLEMENTATION
#include <hyper.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void usage(void);

char** 
GetArgs(
    char                *a_str, 
    char                a_delim, 
    size_t              *count
);

extern int isConnected;

#endif
