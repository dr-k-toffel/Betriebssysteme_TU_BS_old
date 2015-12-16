#include <string.h>
#include "mymem.h"

#define MAGIC ((void*) 0xdeadbeef)  // Unsere Magicnumber zum markieren von belegten Speicherbereichen
#define MEMSIZE 1024 				// Die Größe unseres Speichers in Byte, 1024 => 1 KiB

/**
* Unsere Speicherblockverwaltungseinheit (memblock), speichert wieviele Speicher nach dieser Einheit bis zur nächsten frei ist
* und speichert einen Zeiger auf die nächste freie Einheit
*/
typedef struct memblock {
	size_t size;			//Größe des Freien Speichers der direkt nach dieser Verwaltungseinheit kommt
	struct memblock *next;	//Zeiger auf die nächste Verwaltungseinheit eines Freien Speicherbereichs
} memblock;

static char memory[MEMSIZE];// Unser Speicher, ein einfaches Bytearray (anm: sizeof(char)=1Byte)
memblock *fsp = NULL; 		// Ein Zeiger auf den ersten memblock der frei ist (Free-Space-Pointer)



void *myMalloc(size_t size) {
	// \brief: 0) wenn fsp noch null ist, speicher initialisieren und fsp auf 1. memblock setzen
	// START TODO
		/**
		* Wenn zum ersten mal aufgerufen wird, also fsp noch NULL ist, muss fsp gesetzt werden.
		* Da der Speicher zu diesen Zeitpunkt noch nicht angefasst wurde, muss fsp auf den Anfang des Speichers zeigen.
		* Weiterhin muss dann fsp mit passenden Werten initialisiert werden.
		*/
	if (!fsp) // wenn fsp null war // test test 
	{
		fsp = (memblock*)memory;
		fsp->size = MEMSIZE - sizeof(memblock);
		fsp->next = NULL;
	}
	// END TODO

	// Ein paar Hilfzeiger zum Suchen von freiem Speicher
	memblock *cur = fsp; 	// der block auf dem ich atm arbeite
	memblock *prev = NULL;// der elter block dem dem ich atm arbeite

	// \brief: 1) Wir müssen jetzt einen passenden freien Speicherbereich finden
	while (1) {
		// START TODO	
			/*
			* Ein passender Bereich ist entweder exakt so groß wie der angefordete Speicher
			* oder mindestens so groß, dass der angefordete Speicher hineinpasst
			* plus ein neuer memblock inkl freiem Speicher hinter dem jetzt allokiertem.
			* Der Zeiger cur soll dann auf den passenden freien Speicher zeigen.
			* Der Zeiger prev soll dann auf den Vorgänger von cur zeigen.
			* Wenn eine Stelle mit genügend Speicher gefunden wurde soll die Schleife verlassen werden.
			* Falls hingegen nicht mehr genügend Speicher am Stück bereitsteht, 
			* soll errno auf ENOMEM gesetzt und die Funktion verlassen werden.
			*/	

		if (cur->size >= size) // genug speicher da
		{
			break;
		}
		else // versuche weiter genug speicher zu finden
		{
			prev = cur;
			cur = cur->next;
		}

		if (!cur) // es gab keinen speicherbereich, der gross genug war
		{
			errno = ENOMEM;
			perror("malloc");
			return NULL;
		}
		// END TODO
	}

	//Variante2 (s. Übung): zu Reservierende Speichermenge als aufgerundetes Vielfache von memblock ausrechnen
	int units = ((size - 1) / sizeof(memblock)) + 1; 
		
	size_t curoldsize = cur->size;
	memblock *curoldnext = cur->next;

	memblock *newPtr;
	
	// \brief: 2) Speicher Reservieren (entsprechende infos im aktuellen memblock eintragen) 
	// START TODO
		/**
		* Die Größe von cur muss auf die zu reservierende speichermenge gesetzt werden 
		* und der Speicher soll als belegt markiert werden.
		* Der newPtr soll dann auf die Stelle im Speicher zeigen wo der nächste memblock abgelegt wird
		*/
		cur->size = size;
		cur->next = MAGIC;
		newPtr = (memblock*)((char*)(cur + units + 1));
	// END TODO

	// \brief: 3) Nächsten Memblock Ablegen 
	// START TODO
		/**
		* Wenn der alte Nachfolger von cur NICHT unser newptr ist, dann muss an der Stelle wo newPtr hinzeigt
		* ein neuer memblock initialisiert werden. Größe muss passend berechnet werden und der alte Nachfolger von
		* cur wird nun Nachfolger von newPtr.
		* Wenn der alte Nachfolger von cur und newPtr auf die gleiche Stelle zeigen, liegt dort bereits ein memblock
		* und dieser darf dann NICHT verändert werden.
		*/
		if ((((void*)newPtr) < ((void*)&(memory[MEMSIZE-1]))) && (newPtr != curoldnext)) // testen ob wir noch im array sind und ob newPtr nicht curnext war
		{
			newPtr->size = curoldsize - (sizeof(memblock) * (units + 1));
			newPtr->next = curoldnext;
		}
	// END TODO

	// \brief: 4) fsp verschieben.
	// START TODO
		/**
		* Wenn der Vorgänger NULL ist, also wir den ersten freien Speicher nehmen den wir bekamen,
		* dann muss fsp auf diesen neuen memblock zeigen. Andernfalls muss der Nachfolger vom Vorgänger
		* auf den neuen Speicher zeigen
		*/
		if (cur == fsp)
		{
			fsp = newPtr;
		}
		else
		{
			prev->next = newPtr;
		}
	// END TODO

	// Am Ende geben wir einen Pointer auf den Anfang des jetzt passenden Speichers zurück
	return (void*) ((char*) cur + sizeof(memblock));
}

void myFree(void* ptr) {
	memblock *mbp = NULL; // MemBlockPointer
	
	//  \brief: 1) Adresse des mbp berechnen
	// START TODO
		/**
		* Wir berechnen aus dem gegebenen Zeiger die Adresse des dazugehörenden memblocks 
		* und speichern diese in mbp
		*/
	mbp = (memblock*)((char*)ptr - sizeof(memblock));
	if (((void*)mbp < (void*)memory) || ((void*)mbp > (void*)&(memory[MEMSIZE - 1])))
	{
		printf("die Adresse %p, liegt nicht im verwalteten Speicherbereich", (void*)mbp);
		return;
	}
	// END TODO

	// \brief: 2) Prüfen ob der Speicher wirklich belegt ist		
	// START TODO
		/**
		* Wir testen ob dieser Berech auch tatsächlich belegt ist, 
		* wenn nicht brechen wir ab und setzen errno auf ENOMEM
		*/
	if (mbp->next != MAGIC)
	{
		errno = ENOMEM;
		perror("free");
		return;
	}
	// END TODO

	// \brief: 3) fsp setzen und evtl umliegende memblöcke aktualisieren
	// START TODO
		/**
		* Wenn unser gefundener memblock vor unserem aktuell erstem freien Speicher (fsp) liegt
		* dann muss der Nachfolger vom gefundenem memblock auf den aktuell freien zeigen
		* und der aktuell erste freie wird unser gefundener memblock.
		*
		* Liegt der gefundene memblock jedoch hinter dem erstem freien Speicher (fsp),
		* dann kann man davon ausgehen, dass der mbp sich zwichen zwei freien Speicherbereichen befindet.	
		* In diesem Fall müssen wir den memblock (mbp_pre) suchen der vor mbp liegt 
		* und dessen Nachfolger (mbp_past) sich hinter mbp befindet.
		* Anschließend müssen die next Zeiger der memblöcke korrekt gesetzt werden.
		*/
	if (!fsp) // falls fsp null ist
	{
		fsp = mbp;
		return;
	}
	if (mbp < fsp) // wenn der freie speicher vor fsp ist
	{
		mbp->next = fsp;
		fsp = mbp;
		return;
	}
	memblock* mbp_pre = fsp;
	memblock* mbp_past = mbp_pre->next;
	while(mbp_past)
	{
		if ((mbp_pre < mbp) && (mbp_past > mbp)) // mbp ist zwischen beiden mbp_pre und mbp_past
		{
			break;
		}
		mbp_pre = mbp_past;
		mbp_past = mbp_pre->next;
	}

	mbp_pre->next = mbp;
	mbp->next = mbp_past;
	// END TODO
}

void *myRealloc(void *ptr, size_t size) {
	// OPTIONAL TODO: EINFACHES REALLOC
		/**
		* Normalerweise versucht realloc den Speicherbereich den es bekommt auf Größe size zu vergrößern
		* Da dies kompliziert ist, sollt ihr nur das einfaches realloc machen welches folgende drei Schritte macht:
		* 1. Anfordern eines neuen Speicherbereiches mit Größe size (s. malloc() )
		* 2. Kopieren des Inhaltes vom gegebenem Speicher (s. memcpy() )
		* 3. Freigeben des übergebenem Speichers (.s. free() )
		* Danach wird der Zeiger auf den neuen Bereich zurückgegeben.
		*/
		void* newspace = myMalloc(size);
		memcpy(newspace, ptr, (((memblock*)ptr) - 1)->size); // den size des alten memblock holen, da memcpy wissen muss wie viel es zu kopieren hat
		myFree(ptr);
		return newspace;
	// END TODO
}

void *myCalloc(size_t count, size_t size) {
	// OPTIONAL TODO
		/**
		* calloc ist malloc sehr ähnlich. calloc allokiert Speicher der Größe size count-mal und initialisiert diesen mit 0.
		* Der allokierte Speicher liegt dabei am Stück vor und es wird nur ein Zeiger auf den Anfang zurückgegeben
		* Implementieren sie diese Funktion mit Hilfe von myMalloc
		*/
	size_t i = size * count;
	void* t = myMalloc(i);
	if ( !t ) {
		return t;
	}
	return memset(t, 0, i);
	// END TODO 
}

void printMem() {
	printf("%p : ", (void*) &memory);
	for (int i = 0; i < MEMSIZE; i++) {
		if (i != 0 && (i % 4) == 0) {
			printf(" ");
		}
		if (i != 0 && (i % 32) == 0) {
			printf("\n");
			printf("%p : ", ((char*) &memory) + i);
		}
		printf("%x%x", (memory[i] >> 4) & 0x0f, memory[i] & 0x0f);
	}
	printf("\nfsp = %p\n", (void*) fsp);
}
