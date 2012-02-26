#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "sequence.h"
#include "annotation.h"
#include "collection.h"

/**************************
	create/destroy
***************************/

DNAComponent* createComponent(const char* id) {
	DNAComponent* com;
	com = (DNAComponent*)malloc(sizeof(DNAComponent));
	com->id          = NULL;
	com->name        = NULL;
	com->description = NULL;
	com->numAnnotations = 0;
	com->numCollections = 0;
	com->annotations = NULL;
	com->collections = NULL;
	setComponentID(com, id);
	return com;
}

void deleteComponent(DNAComponent* com) {
	if (com) {
		if (com->id)          free(com->id);
		if (com->name)        free(com->name);
		if (com->description) free(com->description);
		if (com->annotations) free(com->annotations);
		if (com->collections) free(com->collections);
		free(com);
	}
}

/**************************
	getNum... functions
***************************/

int getNumCollectionsFor(const DNAComponent* com) {
	if (com)
		return com->numCollections;
	else
		return NULL;
}

int getNumSequenceAnnotationsIn(const DNAComponent* com) {
	if (com)
		return com->numAnnotations;
	else
		return NULL;
}

/**************************
	getNth... functions
***************************/

struct _Collection* getNthCollectionFor(const DNAComponent* com, int n) {
	if (com->numCollections >= n)
		return com->collections[n];
	else
		return NULL;
}

struct _SequenceAnnotation* getNthSequenceAnnotationIn(const DNAComponent* com, int n) {
	if (com->numAnnotations >= n)
		return com->annotations[n];
	else
		return NULL;
}

/**************************
	set... functions
***************************/

void setComponentID(DNAComponent* com, const char* id) {
	if (com && id) {
		com->id = (char*)malloc(sizeof(char) * strlen(id)+1);
		strcpy(com->id, id);
	}
}

void setComponentName(DNAComponent* com, const char* name) {
	if (com && name) {
		com->name = (char*)malloc(sizeof(char) * strlen(name)+1);
		strcpy(com->name, name);
	}
}

void setComponentDescription(DNAComponent* com, const char* descr) {
	if (com && descr) {
		com->description = (char*)malloc(sizeof(char) * strlen(descr)+1);
		strcpy(com->description, descr);
	}
}

/**************************
	add annotation
***************************/

void addSequenceAnnotation(DNAComponent* com, SequenceAnnotation* ann) {
	if (!com || !ann)
		return;
	ann->annotates = com;
	if (!com->annotations) {
		// create array
		size_t memory = sizeof(SequenceAnnotation*) * 2;
		com->annotations = (SequenceAnnotation**)malloc(memory);
		// add first value
		com->annotations[0] = ann;
		// finish with null
		com->annotations[1] = NULL;
	} else {
		// create longer array
		int n = com->numAnnotations;
		SequenceAnnotation** old = com->annotations; // TODO memory leak?
		size_t memory = sizeof(SequenceAnnotation*) * (n+2);
		com->annotations = (SequenceAnnotation**)malloc(memory);
		// copy over old values
		int i;
		for (i=0; i<n; ++i)
			com->annotations[i] = old[i];
		// add the new one
		com->annotations[n] = ann;
		// finish with NULL
		com->annotations[n+1] = NULL;
	}
	com->numAnnotations++;
}

void setSubComponent(SequenceAnnotation* ann, DNAComponent* com) {
	if (ann && com)
		ann->subComponent = com;
}
