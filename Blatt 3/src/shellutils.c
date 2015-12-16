#include <stdlib.h>   
#include <string.h>  
#include <limits.h>
#include <stdio.h> 
#include <sys/wait.h>
#include <unistd.h> 

#include "plist.h"
#include "shellutils.h"

/* Sie können diese Vorgabe unverändert verwenden oder sie können ihre
 * shellutils.c aus Aufgabe_R1 nutzen. Falls Sie einen Bug in
 * dieser Implementierung finden, melden Sie diesen bitte an
 */

COMMAND *parseCommandLine(char *cmdLine) 
{
	COMMAND *cmd;
	size_t cmdsize; 
	size_t realsize;
	size_t len = strlen(cmdLine);
	size_t ceillen;
	int argc = 0;

	// strip newline
	if(cmdLine[len-1] == '\n') 
	{
		cmdLine[--len] = '\0';
	}

	// alloc command structure
	ceillen = ((len+1)/sizeof(int) + 1) * sizeof(int); // ceil to sizeof(int)
	cmdsize = sizeof(*cmd);
	cmdsize += ceillen;              // for cmdLine copy
	cmdsize += sizeof(char*);        // for argv array
	cmdsize += sizeof(char*);        // for NULL terminator in argv
	realsize = cmdsize + 128;

	cmd = calloc(1, realsize);
	if(cmd == NULL) 
	{ 
		return NULL; 
	}

	// create copy of tokenized command line
	char *cmdLineCopy = ((char*)cmd)+sizeof(*cmd);
	memcpy(cmdLineCopy, cmdLine, len+1);
	cmd->cmdLine = cmdLineCopy;

	char** argvPos = (char**)(((char*)cmd)+sizeof(*cmd)+ceillen);
	for(char *atok=strtok(cmdLine," \t"); atok; atok=strtok(NULL," \t")) 
	{
		cmdsize += sizeof(char*);
		if(cmdsize > realsize) 
		{
			realsize = cmdsize + 128;
			COMMAND *ncmd = realloc(cmd, realsize);
			if(ncmd == NULL) 
			{
				free(cmd); return NULL; 
			}
			argvPos = (char**)ncmd + (argvPos-(char**)cmd); // adjust to new position
			cmd=ncmd;
		}
		*(argvPos++) = atok;
		argc++;
	}
	*argvPos = NULL;

	// Regenerate cmd structure
	cmdLineCopy = ((char*)cmd)+sizeof(*cmd);
	cmd->cmdLine = cmdLineCopy;

	cmd->argv = (char**) (cmdLineCopy+ceillen);

	// check for empty command
	if( argc < 1) 
	{
		cmd->parseError = "empty command line";
		return cmd;
	}

	if(strcmp(*(argvPos-1), "&") == 0) 
	{
		cmd->background = 1;
		argc--;
		if( argc < 1 ) 
		{
			cmd->parseError = "background execution of empty command requested";
			return cmd;
		}
		argvPos--;
		*argvPos = NULL;
	}

	if ( (*(argvPos-1))[0] == '>' ) 
	{
		cmd->outFile = &(*(argvPos-1))[1];

		if ( strlen(cmd->outFile) < 1 ) 
		{
			cmd->parseError = "stdout redirection to zero length filename requested";
			return cmd;
		}

		argc--;
		if( argc < 1 ) 
		{
			cmd->parseError = "stdout redirection of empty command requested";
			return cmd;
		}
		argvPos--;
		*argvPos = NULL;
	}

	return cmd;
}

/* prints status of terminated child processes */
void printStat(const char *cmdLine, int status) 
{
	if (WIFSIGNALED(status)) 
	{
		fprintf(stderr,"Signal [%s] = %d\n",cmdLine,WTERMSIG(status));
	} 
	else if(WIFEXITED(status)) 
	{ 
		fprintf(stderr,"Exitstatus [%s] = %d\n",cmdLine,WEXITSTATUS(status));
	} 
	else if(WIFSTOPPED(status)) 
	{
		fprintf(stderr, "Stopped [%s] = %d\n", cmdLine, WSTOPSIG(status));
	}
}

/* just to make sure there is enough room for [unknown] */
#if PATH_MAX > 10
#	define MAX_PATH_LEN PATH_MAX
#else
#	define MAX_PATH_LEN 10
#endif
void prompt() 
{
	char  cworkdir[MAX_PATH_LEN];
	if (NULL==getcwd(cworkdir, MAX_PATH_LEN)) 
	{
		perror("getcwd");
		strcpy(cworkdir,"[unknown]");
	}
	fprintf(stderr, "%s: ",cworkdir);
}

