#define MAX_INPUT_BUFFER 1024

#include <hyper.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

void usage(void);

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


COMMAND command_list[] = {
    {"SEND", &send_file},
    {"LIST", &list_dir},
    {"QUIT", &client_quit}
};

