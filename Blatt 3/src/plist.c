#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "plist.h"

/* Die Funktionen insertElement() und removeElement() bitte unveraendert lassen!
 * Falls Sie einen Bug in dieser Implementierung finden, melden Sie diesen bitte
 */

static struct qel 
{
	pid_t pid;
	char *cmdLine;
	struct qel *next;
} *head;

void walkList( int (*callback) (pid_t, const char *) ) 	/* 1 abbruch
																													-1 faengt bei head an
																													 0 laeuft normal weiter
																												*/
{
	struct qel *curr = head;
	while (curr) {
		switch ((*callback)(curr->pid, curr->cmdLine))
		{
			case -1 : {
				curr = head;
				break;
			}
			case 0 : {
				curr = curr->next;
				break;
			}
			case 1 : {
				return;
			}
		}
	}
}

int insertElement(pid_t pid, const char *cmdLine) 
{
	struct qel *lauf = head;
	struct qel *schlepp = NULL;

	while ( lauf ) 
	{
		if ( lauf->pid == pid ) 
		{
			return -1;
		}

		schlepp = lauf;
		lauf = lauf->next;
	}

	lauf = malloc(sizeof(struct qel));
	if ( NULL == lauf ) 
	{ 
		return -2; 
	}

	lauf->cmdLine = strdup(cmdLine);
	if( NULL == lauf->cmdLine ) 
	{
		free(lauf);
		return -2;
	}

	lauf->pid  = pid;
	lauf->next = NULL;

	/* Einhaengen des neuen Elements */
	if ( NULL == schlepp ) 
	{
		head = lauf;
	} 
	else 
	{
		schlepp->next = lauf;
	}

	return pid;
}

int removeElement(pid_t pid, char *buf, size_t buflen) 
{
	if ( head == NULL ) 
	{
		return -1;
	}

	struct qel* lauf = head;
	struct qel* schlepp = NULL;

	while ( lauf ) 
	{
		if ( lauf->pid == pid ) 
		{
			if ( NULL == schlepp ) 
			{
				head = head->next;
			} 
			else 
			{
				schlepp->next = lauf->next;
			}

			strncpy(buf, lauf->cmdLine, buflen);
			if( buflen>0 ) 
			{
				buf[buflen-1]='\0';
			}
			int retVal = strlen(lauf->cmdLine);

			/* Speicher freigeben */
			free(lauf->cmdLine);
			free(lauf);
			return retVal;
		}

		schlepp = lauf;
		lauf = lauf->next;
	}

	/* PID not found */ 
	return -1;
}
