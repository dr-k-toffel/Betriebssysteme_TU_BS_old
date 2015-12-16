//BEGIN TODO

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shellutils.h"
#include "plist.h"

int printProcess(pid_t pid, const char * cmdline) /* gibt die pid und die cmdline aus */
{
	fprintf(stderr, "%i \t|\t %s\n", pid, cmdline);
	return 0;
}

int removeZombieProcess(pid_t pid, const char * cmdline) /* entfernt den prozess aus der plist, wenn er beendet wurde */
{
	int status;
	pid_t wait = waitpid(pid, &status, WNOHANG);
	if (wait == 0) // das kind ist nicht fertig
	{
		return 0;
	}
	char buffer[128];
	removeElement(pid, buffer, 128);
	printStat(buffer, status);
	return -1;
}

void runCommandCd(COMMAND* cmd) /* ruft chdir auf und verhaelt sich auch so */
{
	if (cmd->argv[1] != NULL ) // prueft ob der erste parameter existiert
	{
		if (chdir(cmd->argv[1]) == -1) // wenn ein fehler auftritt
		{
			perror("chdir");
		}
		if (cmd->argv[2] != NULL ) // gibt eine warnung aus falls es mehr als einen parameter gibt aber ignoriert diese
		{
			fprintf(stderr, "%s", "chdir: use only the first parameter\n");
		}
	}
	else // wenn der erste parameter null ist verweigert er die eingabe
	{
		fprintf(stderr, "%s", "chdir: need the first parameter\n");
	}
}

void runCommandJobs(COMMAND* cmd) /* gibt alle laufenden prozesse aus */
{
	fprintf(stderr, "%s", "list of all running process\n");
	walkList(&printProcess);
}

void runCommand(COMMAND* cmd) /* startet das angegebene programm mit der gegebenen konfiguration */
{
	int status;
	pid_t cpid;
	cpid = fork();
	if (cpid == -1) // fehler beim forken
	{
		perror("fork");
		exit(-1);
	}
	else if (cpid == 0) // das kind erhaelt die cpid 0 also macht das hier das kind
	{
		if (execvp(cmd->argv[0], cmd->argv) == -1)
		{ // fehler beim starten des programms im kindprozess
			perror("execvp");
			exit(-1);
		}
		exit(0);
	}
	else // der elter sieht die richtige cpid somit macht das hier der elter
	{
		insertElement(cpid, cmd->cmdLine);
		if (!cmd->background) // wenn das kind im background laufen soll wartet der elter nicht
		{
			pid_t wait = waitpid(cpid, &status, WUNTRACED);
			if (wait == -1) // fehler
			{
				perror("waitpid");
				exit(-1);
			}
			char buffer[128];
			removeElement(cpid, buffer, 128);
			printStat(buffer, status);
		}
	}
}

void checkEndedChildren() /* prueft alle beendeten kinder und loescht diese aus der plist */
{
	walkList(&removeZombieProcess);
}

int main()
{
	fprintf(stderr, "%s", "Hello, Master\n");
	COMMAND* pcmd;
	char buffer[128];
	do // prueft die ganze zeit die eingaben
	{
		checkEndedChildren();
		prompt();
		fgets(buffer, 128, stdin);
		if (!feof(stdin) && ((char*) buffer[0]) != NULL ) // mach nichts wenn es unsinnig ist.
		{
			pcmd = parseCommandLine(buffer);
			if ((strcmp(pcmd->argv[0], "cd") == 0) || (strcmp(pcmd->argv[0], "chdir") == 0))
			{ // teste ob cd als cmd gegeben wurde
				runCommandCd(pcmd);
			}
			else if ((strcmp(pcmd->argv[0], "jobs") == 0))
			{
				runCommandJobs(pcmd);
			}
			else
			{ // es wurde ein cmd gegeben bei dem kein vorgegebenen programm benutzt werden soll
				runCommand(pcmd);
			}
		}
	}
	while (!feof(stdin) && ((char*) buffer[0]) != NULL ); // falls fgets ein null schreibt
	fprintf(stderr, "%s", "\nbye bye\n");
	exit(0);
}

//END TODO
