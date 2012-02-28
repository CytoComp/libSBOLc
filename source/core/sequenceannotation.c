#include <stdlib.h>
#include <string.h>
#include "genericarray.h"
#include "sequenceannotation.h"

static GenericArray* allSequenceAnnotations;

/*******************
 * create/destroy
 *******************/

void registerSequenceAnnotation(SequenceAnnotation* ann) {
	if (!allSequenceAnnotations)
		allSequenceAnnotations = createGenericArray();
	insertIntoGenericArray(allSequenceAnnotations, ann);
}

SequenceAnnotation* createSequenceAnnotation(const char* id) {
	SequenceAnnotation* ann;
	ann = (SequenceAnnotation*)malloc(sizeof(SequenceAnnotation));
	ann->id           = NULL;
	ann->genbankStart = 0;
	ann->genbankEnd   = 0;
	ann->numPrecedes  = 0;
	ann->annotates    = NULL;
	ann->subComponent = NULL;
	ann->precedes     = NULL;
	setSequenceAnnotationID(ann, id);
	registerSequenceAnnotation(ann);
	return ann;
}

void removeSequenceAnnotation(SequenceAnnotation* ann) {
	if (ann && allSequenceAnnotations) {
		int index = indexByPtr(allSequenceAnnotations, ann);
		if (index >= 0)
			removeFromGenericArray(allSequenceAnnotations, index);
	}
}

void deleteSequenceAnnotation(SequenceAnnotation* ann) {
	if (ann) {
		removeSequenceAnnotation(ann);
		if (ann->id) {
			free(ann->id);
			ann->id = NULL;
		}
		// TODO will these delete parts of other structs?
		if (ann->annotates) {
			free(ann->annotates);
			ann->annotates = NULL;
		}
		if (ann->subComponent) {
			free(ann->subComponent);
			ann->subComponent = NULL;
		}
		if (ann->precedes) {
			free(ann->precedes);
			ann->precedes = NULL;
		}
		free(ann);
	}
}

void setSequenceAnnotationID(SequenceAnnotation* ann, const char* id) {
	if (ann) {
		if (!ann->id)
			ann->id = (char*)malloc(sizeof(char) * strlen(id)+1);
		strcpy(ann->id, id);
	}
}

/*******************
 * is... functions
 *******************/

int isAnnotationPtr(const void* pointer) {
	if (!allSequenceAnnotations)
		allSequenceAnnotations = createGenericArray();
	return (int) indexByPtr(allSequenceAnnotations, pointer) >= 0;
}

int isSequenceAnnotationID(const char* id) {
	if (!allSequenceAnnotations)
		allSequenceAnnotations = createGenericArray();
	if (!id)
		return 0;
	int index;
	SequenceAnnotation* ann;
	for (index=0; index<allSequenceAnnotations->numInUse; index++) {
		ann = (SequenceAnnotation*) allSequenceAnnotations->array[index];
		if (strcmp(ann->id, id) == 0)
			return 1;
	}
	return 0;
}

/***********************
 * getNum... functions
 ***********************/

int getNumSequenceAnnotations() {
	if (!allSequenceAnnotations)
		allSequenceAnnotations = createGenericArray();
	return allSequenceAnnotations->numInUse;
}

SequenceAnnotation* getNthSequenceAnnotation(int n) {
	if (!allSequenceAnnotations)
		allSequenceAnnotations = createGenericArray();
	if (allSequenceAnnotations->numInUse > n)
		return allSequenceAnnotations->array[n];
	else
		return NULL;
}

/********************
 * get... functions
 ********************/

SequenceAnnotation* getSequenceAnnotation(const char* id) {
	if (!allSequenceAnnotations)
		allSequenceAnnotations = createGenericArray();
	if (!id)
		return NULL;
	int index;
	SequenceAnnotation* ann;
	for (index=0; index<allSequenceAnnotations->numInUse; index++) {
		ann = (SequenceAnnotation*) allSequenceAnnotations->array[index];
		if (ann->id == id)
			return ann;
	}
	return NULL;
}

/*******************
 * constrain order
 *******************/

void addPrecedesRelationship(SequenceAnnotation * upstream, SequenceAnnotation * downstream) {
	if (!upstream || !downstream)
		return;
	if (!upstream->precedes) {
		// create array
		size_t size = sizeof(SequenceAnnotation*) * 2;
		upstream->precedes = (SequenceAnnotation**)malloc(size);
		// add first value
		upstream->precedes[0] = downstream;
		// add null terminator
		upstream->precedes[1] = NULL;
	} else {
		// create longer array
		int n = upstream->numPrecedes;
		SequenceAnnotation** old = upstream->precedes; // TODO memory leak?
		size_t size = sizeof(SequenceAnnotation*) * (n+2);
		upstream->precedes = (SequenceAnnotation**)malloc(size);
		// copy over old values
		int i;
		for (i=0; i<n; ++i)
			upstream->precedes[i] = old[i];
		// add the new one
		upstream->precedes[n] = downstream;
		// finish with NULL
		upstream->precedes[n+1] = NULL;
	}
	upstream->numPrecedes++;
}

int getNumPrecedes(SequenceAnnotation* ann) {
	if (ann)
		return ann->numPrecedes;
	else
		return -1;
}

SequenceAnnotation* getNthPrecedes(SequenceAnnotation* ann, int n) {
	if (ann->numPrecedes >= n)
		return ann->precedes[n];
	else
		return NULL;
}

void cleanupSequenceAnnotations() {
	if (allSequenceAnnotations) {
		int index;
		for (index=allSequenceAnnotations->numInUse; index>0; index--)
			deleteSequenceAnnotation( allSequenceAnnotations->array[index] );
		deleteGenericArray(allSequenceAnnotations);
		allSequenceAnnotations = NULL;
	}
}
