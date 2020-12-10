#include "hyper_server.h"

int isConnected = 0;

void print_ascii(void)
{
    puts( 
        "    __  __                     _____                               \n"
        "   / / / /_  ______  ___  ____/ ___/___  ______   _____  _____     \n"
        "  / /_/ / / / / __ \\/ _ \\/ ___|__ \\/ _ \\/ ___/ | / / _ \\/ ___/\n"
        " / __  / /_/ / /_/ /  __/ /  ___/ /  __/ /   | |/ /  __/ /         \n"
        "/_/ /_/\\__  / ____/\\___/_/  /____/\\___/_/    |___/\\___/_/      \n"
        "      /____/_/                                                     \n"
    );

}

void usage(void)
{
    print_ascii();
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

void server_init(void)
{
    char hostedDir[] = "hosted";

    if (mkdir(hostedDir, 0700) == 0 || errno == EEXIST)
        chdir(hostedDir);
    else
    {
        puts("[-] Couldn't make hosted directory");
        return;
    }

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
    
    print_ascii();

    server_init();

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

    while (1)
    {
        isConnected = 0;
        iResult = HyperServerListen(sockServer, &sockClient);
        if (iResult != HYPER_SUCCESS)
        {
            printf("[-] HyperServerListen failed\n");
            return HYPER_FAILED;
        }
        else
            printf("[*] Client connected\n");

        isConnected = 1;
        while (isConnected == 1)
        {
            memset(command, 0, MAX_INPUT_BUFFER);

            iResult = HyperReceiveCommand(sockClient, command, MAX_INPUT_BUFFER);
            if (iResult != HYPER_SUCCESS)
            {
                printf("[-] HyperRecieveCommand failed\n");
                return HYPER_FAILED;
            }
            else
                printf("[+] Command recieved. %s\n", command);

            command_handler(sockClient, command);
        }
    }
        
    HyperCloseSocket(sockServer);
    HyperCloseSocket(sockClient);
    HyperSocketCleanup();
    return HYPER_SUCCESS;
}
