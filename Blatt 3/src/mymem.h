#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
Allokiert Speicher der Größe size und gibt einen Pointer auf diesen Speicherbereich zurück
*/
void *myMalloc(size_t size);

/*
Gibt einen von myMalloc allokierten Speicherbereich (ptr) wieder frei.
*/
void myFree(void *ptr);

/*
Vergrößert einen von myMalloc allokierten Speicherbereich (ptr) auf Größe size
*/
void *myRealloc(void *ptr, size_t size);

/*
Allokiert wie myMalloc einen Speicherbereich der Größe size, diesen aber count-mal
*/
void *myCalloc(size_t count, size_t size);

/*
Gibt den gesamten Speicherbereich auf der Konsole aus
*/
void printMem();

#define malloc(size) myMalloc(size)
#define free(ptr) myFree(ptr)
#define realloc(ptr, size) myRealloc(ptr, size);
#define calloc(count, size) myCalloc(count, size);