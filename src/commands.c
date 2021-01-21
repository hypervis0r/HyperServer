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
<<<<<<< HEAD
    /* Unimplemented for now */
=======
    DIR *dpDir = NULL;
    struct dirent *entry = {0};
    struct stat st = {0};
    char *cpDirToList = NULL;
    
    char *entryBuffer = NULL;
    char *listBuffer = NULL;
    size_t stListBufferSize = 0;
    size_t stLength = 0;
    
    char filePerms[11];
    memset(filePerms, 0, sizeof(filePerms));

    if (argc > 1)
        cpDirToList = (char*)argv[1];
    else
        cpDirToList = ".";

    dpDir = opendir(cpDirToList);

    if (dpDir)
    {
        HyperSendStatus(sock, 200);

        entry = readdir(dpDir);
        while (entry)
        {
            memset(filePerms, 0, sizeof(filePerms));

            stat(entry->d_name, &st);
            mode_t perm = st.st_mode;

            filePerms[0] = S_ISDIR(perm)    ? 'd' : '-';
            filePerms[1] = (perm & S_IRUSR) ? 'r' : '-';
            filePerms[2] = (perm & S_IWUSR) ? 'w' : '-';
            filePerms[3] = (perm & S_IXUSR) ? 'x' : '-';
            filePerms[4] = (perm & S_IRGRP) ? 'r' : '-';
            filePerms[5] = (perm & S_IWGRP) ? 'w' : '-';
            filePerms[6] = (perm & S_IXGRP) ? 'x' : '-';
            filePerms[7] = (perm & S_IROTH) ? 'r' : '-';
            filePerms[8] = (perm & S_IWOTH) ? 'w' : '-';
            filePerms[9] = (perm & S_IXOTH) ? 'x' : '-';
        
            stLength = snprintf(NULL, 0, "%s %ld %s\n", filePerms, st.st_size, entry->d_name);
            HyperMemRealloc((void**)&entryBuffer, stLength+1);
            stLength = snprintf(entryBuffer, stLength+1, "%s %ld %s\n", filePerms, st.st_size, entry->d_name);

            stListBufferSize += stLength + 1;
            HyperMemRealloc((void**)&listBuffer, stListBufferSize);
            strncat(listBuffer, entryBuffer, stListBufferSize);

            entry = readdir(dpDir);
        }
        
        listBuffer[stListBufferSize] = 0;
        HyperSendCommand(sock, listBuffer);
    }
    else
    {
        HyperSendStatus(sock, 404);
        return;
    }
>>>>>>> Added LIST command | probably some other stuff too idfk
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


