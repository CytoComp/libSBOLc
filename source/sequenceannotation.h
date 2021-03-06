///////////////////////////////////////////////////////////
/// @file
/// This file contains the SequenceAnnotation struct,
/// as well as functions for operating on it.
///////////////////////////////////////////////////////////

#ifndef SBOL_SEQUENCEANNOTATION_HEADER
#define SBOL_SEQUENCEANNOTATION_HEADER

#include "constants.h"
#include "prototypes.h"

/// Implements the SBOL Core SequenceAnnotation object.
struct _SequenceAnnotation {
	Document* doc;                  ///< parent Document
	SBOLObject* base;               ///< uri, displayID, name, description
	PositionProperty *genbankStart; ///< beginning of the annotated feature
	PositionProperty *genbankEnd;   ///< end of the annotated feature
	PolarityProperty *strand;       ///< relative orientation of the annotated feature
	DNAComponent* subComponent;     ///< DNAComponent corresponding to the annotated feature
	PointerArray* precedes;         ///< array of other SequenceAnnotations that must come after this one
};

/// Create an empty SequenceAnnotation.
SBOLAPIEXPORTS SequenceAnnotation* createSequenceAnnotation(Document* doc, const char* uri);

/// Get the URI associated with a SequenceAnnotation.
/// @return A string that must be free()d separately.
SBOLAPIEXPORTS char* getSequenceAnnotationURI(const SequenceAnnotation* ann);

/// Delete a SequenceAnnotation.
/// This doesn't delete any of the other structs it references.
SBOLAPIEXPORTS void deleteSequenceAnnotation(SequenceAnnotation* ann);

/// Find out where on the annotated DNAComponent this feature starts.
SBOLAPIEXPORTS int getSequenceAnnotationStart(const SequenceAnnotation* ann);

/// Find out where on the annotated DNAComponent this feature ends.
SBOLAPIEXPORTS int getSequenceAnnotationEnd(const SequenceAnnotation* ann);

/// Set where on the annotated DNAComponent this feature starts.
SBOLAPIEXPORTS void setSequenceAnnotationStart(SequenceAnnotation* ann, int start);

/// Set where on the annotated DNAComponent this feature ends.
SBOLAPIEXPORTS void setSequenceAnnotationEnd(SequenceAnnotation* ann, int end);

/// Set the orientation of this feature relative to the annotated DNAComponent.
SBOLAPIEXPORTS void setSequenceAnnotationStrand(SequenceAnnotation* ann, int polarity);

/// Find out the orientation of this feature relative to
/// the annotated DNAComponent.
/// @return An int corresponding to one of the values in the StrandPolarity enum,
///         or -1 on failure.
SBOLAPIEXPORTS int getSequenceAnnotationStrand(const SequenceAnnotation* ann);

/// Get the DNAComponent corresponding to the annotated feature.
SBOLAPIEXPORTS DNAComponent* getSequenceAnnotationSubComponent(const SequenceAnnotation* ann);

/// Specify that one SequenceAnnotation must appear before another along a strand of DNA.
/// @todo Is the strand a DNAComponent? Or not yet part of SBOL?
/// @todo how should this handle docs?
SBOLAPIEXPORTS void addPrecedesRelationship(SequenceAnnotation* upstream, SequenceAnnotation* downstream);

/// Find out how many other SequenceAnnotations are restricted to coming after this one.
/// Useful as a loop condition.
SBOLAPIEXPORTS int getNumPrecedes(const SequenceAnnotation* ann);

/// Get the Nth SequenceAnnotation that must be placed after this one.
SBOLAPIEXPORTS SequenceAnnotation* getNthPrecedes(const SequenceAnnotation* ann, int n);

/// Find out whether one SequenceAnnotation is restricted to appearing before another one.
/// @param upstream The SequenceAnnotation that presumed to come first.
/// @param downstream The SequenceAnnotation that presumed to come second.
/// @return Whether upstream actually precedes downstream.
/// @todo how should this handle docs?
SBOLAPIEXPORTS int precedes(const SequenceAnnotation* upstream, const SequenceAnnotation* downstream);

/// @todo removePrecedesRelationship

/// Print an outline of a SequenceAnnotation to stdout.
/// Mainly for debugging.
SBOLAPIEXPORTS void printSequenceAnnotation(const SequenceAnnotation* seq, int tabs);

#endif
