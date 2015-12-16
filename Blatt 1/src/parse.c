#include <stdio.h>
#include "shellutils.h"

int main()
{
	printf("Geben Sie ihren Befehl ein. \n");
	char buffer[128];		
	fgets(buffer, 128, stdin);
    COMMAND *cmd = parseCommandLine(buffer);
	printf("background: %i\n", cmd->background);
	printf("outFile: %s\n", cmd->outFile);
	printf("parseError: %s\n", cmd->parseError);
	printf("cmdLine: %s\n", cmd->cmdLine);
	int counter = 0;
	while (counter < 32 && cmd->argv[counter]) // sonst segmantation fault bei 32 elementen im array 
	{
		printf("argv[%i]: %s\n", counter, cmd->argv[counter]);
		counter++;
	}
    return 0;
}
