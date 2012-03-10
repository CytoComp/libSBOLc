#ifndef SBOL_TYPES
#define SBOL_TYPES

#define XMLNS_RDF  "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
#define XMLNS_RDFS "http://www.w3.org/2000/01/rdf-schema#"
#define XMLNS_SO   "http://purl.obolibrary.org/obo/"
#define XMLNS_SBOL "http://sbols.org/v1#"

#define RDF_TYPE XMLNS_RDF "type"

#define SBOL_DNASEQUENCE        XMLNS_SBOL "DNASequence"
#define SBOL_SEQUENCEANNOTATION XMLNS_SBOL "SequenceAnnotation"
#define SBOL_DNACOMPONENT       XMLNS_SBOL "DNAComponent"
#define SBOL_COLLECTION         XMLNS_SBOL "Collection"

// TODO better name for this?
#define SBOL_IN_COLLECTION XMLNS_SBOL "collection"
#define SBOL_PRECEDES      XMLNS_SBOL "precedes"
//#define SBOL_DISPLAYID     XMLNS_SBOL ":"
#define SBOL_NAME          XMLNS_SBOL "name"
#define SBOL_DESCRIPTION   XMLNS_SBOL "description"
#define SBOL_ANNOTATES     XMLNS_SBOL "annotates"
#define SBOL_SUBCOMPONENT  XMLNS_SBOL "subComponent"
#define SBOL_BIOSTART      XMLNS_SBOL "bioStart"
#define SBOL_BIOEND        XMLNS_SBOL "bioEnd"
#define SBOL_NUCLEOTIDES   XMLNS_SBOL "nucleotides"

#endif