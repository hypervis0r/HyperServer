#include "hyper_server.h"

void usage(void)
{
    puts( 
        "    __  __                     _____                               \n"
        "   / / / /_  ______  ___  ____/ ___/___  ______   _____  _____     \n"
        "  / /_/ / / / / __ \\/ _ \\/ ___|__ \\/ _ \\/ ___/ | / / _ \\/ ___/\n"
        " / __  / /_/ / /_/ /  __/ /  ___/ /  __/ /   | |/ /  __/ /         \n"
        "/_/ /_/\\__  / ____/\\___/_/  /____/\\___/_/    |___/\\___/_/      \n"
        "      /____/_/                                                     \n"
    );
    puts("Usage: hyper-server <PORT>");
}

// Get arguments from input, separated by delimiter
char** 
GetArgs(
    char                *a_str, 
    char                a_delim, 
    size_t              *count
)
{
	char** result = 0;
	char* tmp = a_str;
	char* last_comma = 0;
	char delim[2] = {a_delim, 0};

	// Count how many elements will be extracted.
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			(*count)++;
			last_comma = tmp;
		}
		tmp++;
	}

	// Add space for trailing token.
	*count += last_comma < (a_str + strlen(a_str) - 1);

	// Add space for terminating null string so caller
	// knows where the list of returned strings ends. 
	(*count)++;

	result = realloc(result, sizeof(char*) * (*count));

	if (result)
	{
		size_t idx = 0;
		char* token = strtok(a_str, delim);

		while (token)
		{
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		*(result + idx) = 0;
        
        // Decrement count so count is accurate to array size
	    (*count)--;
	}
	
    return result;
}

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

    for(unsigned int i = 0; i < sizeof(command_list); i++)
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

    if (argc < 2)
        return;

    hsResult = HyperReadFile(argv[1], &hfFile, &ulSize);
    if (hsResult == HYPER_FAILED)
        return;
    
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
    return;
}

void 
client_quit(
    SOCKET              sock,
    const char          **argv,
    const size_t        argc)
{
    puts("[!] Client disconnected");
    exit(HYPER_SUCCESS);
    return;
}

int main(int argc, char **argv)
{
    HYPERSTATUS iResult = 0;
    SOCKET sockServer = 0;
    SOCKET sockClient = 0;
    unsigned short usPort = 0;
    char command[MAX_INPUT_BUFFER];
    
    if (argc < 2)
    {
        usage();
        return HYPER_FAILED;
    }
    else
    {
        usPort = (unsigned short)strtoul(argv[1], NULL, 0);
    }
        
    iResult = HyperNetworkInit();
    if (iResult != HYPER_SUCCESS)
    {
        printf("[-] HyperNetworkInit failed\n");
        return HYPER_FAILED;
    }
    else
        printf("[+] Hyper NetAPI Initialized\n");

    iResult = HyperStartServer(&sockServer, usPort);
    if (iResult != HYPER_SUCCESS)
    {
        printf("[-] HyperStartServer failed\n");
        return HYPER_FAILED;
    }
    else
        printf("[+] Hyper Server has been started\n");

    iResult = HyperServerListen(sockServer, &sockClient);
    if (iResult != HYPER_SUCCESS)
    {
        printf("[-] HyperServerListen failed\n");
        return HYPER_FAILED;
    }
    else
        printf("[+] Connection found\n");

    while (1)
    {
        memset(command, 0, MAX_INPUT_BUFFER);

        iResult = HyperRecieveCommand(sockClient, command, MAX_INPUT_BUFFER);
        if (iResult != HYPER_SUCCESS)
        {
            printf("[-] HyperRecieveCommand failed\n");
            return HYPER_FAILED;
        }
        else
            printf("[+] Command recieved. %s\n", command);

        command_handler(sockClient, command);
    }
    
    HyperCloseSocket(sockServer);
    HyperCloseSocket(sockClient);
    HyperSocketCleanup();
    return HYPER_SUCCESS;
}
