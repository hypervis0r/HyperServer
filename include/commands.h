#ifndef _COMMANDS_H
#define _COMMANDS_H

#define SERVER_MAX_PATH 4096 /* I have issues with limits.h so fml */

/* File Perm Masks */
#define S_DIR   01

#include "hyper_server.h"

#include <hyper.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int 
command_handler(
    SOCKET              sock,
    char                *command
);

void 
send_file(
    SOCKET              sock,
    const char          **argv,
    const size_t        argc
);

void 
list_dir(
    SOCKET              sock,
    const char          **argv,
    const size_t        argc
);

void
client_quit(
    SOCKET              sock,
    const char          **argv,
    const size_t        argc
);

typedef void(*FUNCPTR)(
    SOCKET,
    const char**,
    const size_t
);

typedef struct _COMMAND
{
    const char          *command;
    FUNCPTR             execute;
} COMMAND, * PCOMMAND;

extern COMMAND command_list[3];

#endif
