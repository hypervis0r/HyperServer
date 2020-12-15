#include "commands.h"

COMMAND command_list[3] = {
    {"SEND", &send_file},
    {"LIST", &list_dir},
    {"QUIT", &client_quit}
};
unsigned int numCommands = 3;

int command_handler(
    SOCKET              sock,
    char                *command
)
{
    if (command == NULL)
        return HYPER_FAILED;

    size_t stArgsSize = 0;
    char** args = GetArgs(command, ' ', &stArgsSize);
    if (args == NULL)
        return HYPER_FAILED;

    for(unsigned int i = 0; i < sizeof(command_list)-1; i++)
    {
        if (strcmp(command_list[i].command, command) == 0)
        {
            command_list[i].execute(sock, (const char**)args, stArgsSize);
            free(args);
            return HYPER_SUCCESS;
        }
    }

    free(args);
    return HYPER_FAILED;
}

void send_file(
    SOCKET sock,
    const char          **argv,
    const size_t        argc
)
{
    HYPERSTATUS hsResult = 0;
    HYPERFILE hfFile = NULL;
    unsigned long ulSize = 0;
    
    char cpFilePath[SERVER_MAX_PATH];
    char cpCWD[SERVER_MAX_PATH];
    memset(cpFilePath, 0, SERVER_MAX_PATH);
    memset(cpCWD, 0, SERVER_MAX_PATH);

    getcwd(cpCWD, SERVER_MAX_PATH);

    if (argc < 2)
        return;
    
    /* TODO: This is very bad and *will* lead to a directory traversal vuln */ 
    if (realpath(argv[1], cpFilePath) == NULL)
    {
        HyperSendStatus(sock, 404);
        return;
    }
    if (strstr(cpFilePath, cpCWD) == NULL)
    {
        HyperSendStatus(sock, 404);
        return;
    }


    hsResult = HyperReadFile(cpFilePath, &hfFile, &ulSize);
    if (hsResult == HYPER_FAILED)
    {
        HyperSendStatus(sock, 400);
        return;
    }
    
    HyperSendStatus(sock, 200);

    HyperSendFile(sock, &hfFile, ulSize);
    
    HyperMemFree(hfFile);
}

void 
list_dir(
    SOCKET              sock,
    const char          **argv,
    const size_t        argc)
{
    
}

void 
client_quit(
    SOCKET              sock,
    const char          **argv,
    const size_t        argc)
{
    puts("[!] Client disconnected");
    isConnected = 0; 
    return;
}


