%module libsbol
%{
    #define SWIG_FILE_WITH_INIT

    #include <libxml/xmlmemory.h>
    #include <libxml/parser.h>
    #include <libxml/xpath.h>
    #include <libxml/xmlwriter.h>

    #include "constants.h"
    #include "prototypes.h"
    #include "array.h"
    #include "utilities.h"
    #include "collection.h"
    #include "dnacomponent.h"
    #include "dnasequence.h"
    #include "object.h"
    #include "property.h"
    #include "reader.h"
    #include "sequenceannotation.h"
    #include "validator.h"
    #include "writer.h"
    #include "sbol.h"
%}

enum StrandPolarity {
	STRAND_FORWARD,
	STRAND_BIDIRECTIONAL,
	STRAND_REVERSE
};

// tell SWIG how to free strings
%typemap(newfree) char* "free($1);";

// tell SWIG to free the strings returned
// from these functions immediately
// (after copying their values into Python)
%newobject getDNASequenceNucleotides;
%newobject getDNASequenceURI;
%newobject getSequenceAnnotationURI;
%newobject getDNAComponentURI;
%newobject getDNAComponentDisplayID;
%newobject getDNAComponentName;
%newobject getDNAComponentDescription;
%newobject getCollectionURI;
%newobject getCollectionDisplayID;
%newobject getCollectionName;
%newobject getCollectionDescription;

// tell SWIG that these functions delete
// their first argument, and it doesnt need
// to be garbage collected
// (SBOL objects are managed manually anyway,
//  so this just makes doubly sure to avoid segfaults
//  from freeing twice)
%delobject deleteDNASequence;
%delobject deleteSequenceAnnotation;
%delobject deleteDNAComponent;
%delobject deleteCollection;

// functions returing pointers to new objects
DNASequence* createDNASequence(char* uri);
DNAComponent* createDNAComponent(const char* uri);
Collection* createCollection(const char* uri);
SequenceAnnotation* createSequenceAnnotation(const char* uri);

// functions that delete objects
void deleteDNASequence(DNASequence* seq);
void deleteSequenceAnnotation(SequenceAnnotation* ann);
void deleteDNAComponent(DNAComponent* com);
void deleteCollection(Collection* col);

// funtions returning strings to be newfree()d
char* getDNASequenceNucleotides(const DNASequence* seq);
char* getDNASequenceURI(const DNASequence* seq);
char* getSequenceAnnotationURI(const SequenceAnnotation* ann);
char* getDNAComponentURI(const DNAComponent* com);
char* getDNAComponentDisplayID(const DNAComponent* com);
char* getDNAComponentName(const DNAComponent* com);
char* getDNAComponentDescription(const DNAComponent* com);
char* getCollectionURI(const Collection* col);
char* getCollectionDisplayID(const Collection* col);
char* getCollectionName(const Collection* col);
char* getCollectionDescription(const Collection* col);

// functions returning pointers to existing objects
DNASequence* getNthDNASequence(int n);
DNASequence* getDNASequence(const char* uri);
DNASequence* getDNAComponentSequence(DNAComponent* com);
SequenceAnnotation* getSequenceAnnotation(const char* uri);
SequenceAnnotation* getNthSequenceAnnotation(int n);
SequenceAnnotation* getNthPrecedes(const SequenceAnnotation* ann, int n);
SequenceAnnotation* getNthSequenceAnnotationFor(const DNAComponent* com, int n);
DNAComponent* getNthDNAComponent(int n);
DNAComponent* getDNAComponent(const char* uri);
DNAComponent* getSequenceAnnotationSubComponent(const SequenceAnnotation* ann);
DNAComponent* getNthDNAComponentIn(const Collection* col, int n);
Collection* getCollection(const char* uri);
Collection* getNthCollection(int n);

// functions returning ints
int getNumSBOLObjects();
int isSBOLObjectURI(const char* uri);
int getNumDNASequences();
int isDNASequenceURI(const char* uri);
int isSequenceAnnotation(const void* pointer);
int isSequenceAnnotationURI(const char* uri);
int getNumSequenceAnnotations();
int getSequenceAnnotationStart(const SequenceAnnotation* ann);
int getSequenceAnnotationEnd(const SequenceAnnotation* ann);
int getSequenceAnnotationStrand(const SequenceAnnotation* ann);
int getNumPrecedes(const SequenceAnnotation* ann);
int precedes(const SequenceAnnotation* upstream, const SequenceAnnotation* downstream);
int isDNAComponent(const void* pointer);
int isDNAComponentURI(const char* uri);
int getNumDNAComponents();
int getNumSequenceAnnotationsFor(const DNAComponent* com);
int isCollection(const void* pointer);
int isCollectionURI(const char* uri);
int getNumCollections();
int getNumDNAComponentsIn(const Collection* col);
int dnaComponentInCollection(const DNAComponent* com, const Collection* col);
int writeSBOLCore(const char* filename);

// functions returning void
void setDNASequenceNucleotides(DNASequence* seq, const char* nucleotides);
void printDNASequence(const DNASequence* seq, int tabs);
void setSequenceAnnotationStart(SequenceAnnotation* ann, int start);
void setSequenceAnnotationEnd(SequenceAnnotation* ann, int end);
void setSequenceAnnotationStrand(SequenceAnnotation* ann, int polarity);
void addPrecedesRelationship(SequenceAnnotation* upstream, SequenceAnnotation* downstream);
void printSequenceAnnotation(const SequenceAnnotation* seq, int tabs);
void setDNAComponentURI(DNAComponent* com, const char* uri);
void setDNAComponentDisplayID(DNAComponent* com, const char* id);
void setDNAComponentName(DNAComponent* com, const char* name);
void setDNAComponentDescription(DNAComponent* com, const char* desc);
void setDNAComponentSequence(DNAComponent* com, DNASequence* seq);
void addSequenceAnnotation(DNAComponent* com, SequenceAnnotation* ann);
void setSequenceAnnotationSubComponent(SequenceAnnotation* ann, DNAComponent* com);
void printDNAComponent(const DNAComponent* com, int tabs);
void setCollectionDisplayID(Collection* col, const char* id);
void setCollectionName(Collection* col, const char* name);
void setCollectionDescription(Collection* col, const char* desc);
void addDNAComponentToCollection(DNAComponent* com, Collection* col);
void printCollection(const Collection* col, int tabs);
void readSBOLCore(char* filename);
void printSBOLCore();
void cleanupSBOLCore();

