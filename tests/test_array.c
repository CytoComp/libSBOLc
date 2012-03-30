#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"
#include "utilities.h"
#include "array.h"
#include "dnacomponent.h"
#include "sequenceannotation.h"
#include "collection.h"

#define NUM_FAST_TESTS 100
#define NUM_SLOW_TESTS  10

void TestSingleDNAComponent(CuTest* tc) {
	cleanupDNAComponents();
	CuAssertIntEquals(tc, 0, getNumDNAComponents());
	DNAComponent* com = createDNAComponent("one");
	CuAssertIntEquals(tc, 1, getNumDNAComponents());
	CuAssertPtrEquals(tc, com, getNthDNAComponent(0));
	CuAssertIntEquals(tc, 1, isDNAComponentURI("one"));
	CuAssertIntEquals(tc, 1, isDNAComponentPtr(com));
	deleteDNAComponent(com);
	CuAssertIntEquals(tc, 0, getNumDNAComponents());
	CuAssertIntEquals(tc, 0, isDNAComponentURI("one"));
	CuAssertIntEquals(tc, 0, isDNAComponentPtr(com));
}

void TestNumDNAComponents(CuTest* tc) {
	cleanupDNAComponents();
	char* uri;
	DNAComponent* com;
	int num;
	for (num=0; num<NUM_FAST_TESTS; num++) {
		CuAssertIntEquals(tc, num, getNumDNAComponents());
		uri = randomUniqueURI();
		com = createDNAComponent(uri);
		CuAssertIntEquals(tc, num+1, getNumDNAComponents());
	}
	for (; num>0; num--) {
		CuAssertIntEquals(tc, num, getNumDNAComponents());
		com = getNthDNAComponent(0);
		deleteDNAComponent(com);
		CuAssertIntEquals(tc, num-1, getNumDNAComponents());
	}
	CuAssertIntEquals(tc, 0, getNumDNAComponents());
}

void TestDNAComponentIndexing(CuTest* tc) {
	cleanupDNAComponents();
	char* uri;
	DNAComponent* com;
	int num;
	for (num=0; num<NUM_SLOW_TESTS; num++) {
		uri = randomUniqueURI();
		while (isDNAComponentURI(uri))
			uri = randomUniqueURI();
		com = createDNAComponent(uri);
	}
	int index;
	for (num=NUM_SLOW_TESTS; num>0; num--) {
		CuAssertIntEquals(tc, num, getNumDNAComponents());
		index = randomNumber(num);
		com = getNthDNAComponent(index);
		// copy uri (because it will be destroyed)
		uri = getDNAComponentURI(com);
		CuAssertIntEquals(tc, 1, isDNAComponentPtr(com));
		CuAssertIntEquals(tc, 1, isDNAComponentURI(uri));
		deleteDNAComponent(com);
		CuAssertIntEquals(tc, num-1, getNumDNAComponents());
		CuAssertIntEquals(tc, 0, isDNAComponentPtr(com));
		CuAssertIntEquals(tc, 0, isDNAComponentURI(uri));
	}
}

void TestSingleAnnotation(CuTest* tc) {
	cleanupSequenceAnnotations();
	CuAssertIntEquals(tc, 0, getNumSequenceAnnotations());
	SequenceAnnotation* ann = createSequenceAnnotation("one");
	CuAssertIntEquals(tc, 1, getNumSequenceAnnotations());
	CuAssertPtrEquals(tc, ann, getNthSequenceAnnotation(0));
	CuAssertIntEquals(tc, 1, isSequenceAnnotationURI("one"));
	CuAssertIntEquals(tc, 1, isSequenceAnnotation(ann));
	deleteSequenceAnnotation(ann);
	CuAssertIntEquals(tc, 0, getNumSequenceAnnotations());
	CuAssertIntEquals(tc, 0, isSequenceAnnotationURI("one"));
	CuAssertIntEquals(tc, 0, isSequenceAnnotation(ann));
}

void TestNumAnnotations(CuTest* tc) {
	cleanupSequenceAnnotations();
	char* uri;
	SequenceAnnotation* ann;
	int num;
	for (num=0; num<NUM_FAST_TESTS; num++) {
		CuAssertIntEquals(tc, num, getNumSequenceAnnotations());
		uri = randomUniqueURI();
		ann = createSequenceAnnotation(uri);
		CuAssertIntEquals(tc, num+1, getNumSequenceAnnotations());
	}
	for (; num>0; num--) {
		CuAssertIntEquals(tc, num, getNumSequenceAnnotations());
		ann = getNthSequenceAnnotation(0);
		deleteSequenceAnnotation(ann);
		CuAssertIntEquals(tc, num-1, getNumSequenceAnnotations());
	}
	CuAssertIntEquals(tc, 0, getNumSequenceAnnotations());
}

void TestAnnotationIndexing(CuTest* tc) {
	cleanupSequenceAnnotations();
	char* uri;
	SequenceAnnotation* ann;
	int num;
	for (num=0; num<NUM_SLOW_TESTS; num++) {
		uri = randomUniqueURI();
		// avoid duplicates
		while (isSequenceAnnotationURI(uri))
			uri = randomUniqueURI();
		ann = createSequenceAnnotation(uri);
	}
	int index;
	for (num=NUM_SLOW_TESTS; num>0; num--) {
		CuAssertIntEquals(tc, num, getNumSequenceAnnotations());
		index = randomNumber(num);
		ann = getNthSequenceAnnotation(index);
		// copy uri (because it will be destroyed)
		uri = getSequenceAnnotationURI(ann);
		CuAssertIntEquals(tc, 1, isSequenceAnnotation(ann));
		CuAssertIntEquals(tc, 1, isSequenceAnnotationURI(uri));
		deleteSequenceAnnotation(ann);
		CuAssertIntEquals(tc, num-1, getNumSequenceAnnotations());
		CuAssertIntEquals(tc, 0, isSequenceAnnotation(ann));
		CuAssertIntEquals(tc, 0, isSequenceAnnotationURI(uri));
	}
}

void TestCleanupSequenceAnnotations(CuTest* tc) {
	cleanupSequenceAnnotations();
	int num;
	for (num=0; num<NUM_FAST_TESTS; num++)
		createSequenceAnnotation( randomUniqueURI() );
	CuAssertIntEquals(tc, NUM_FAST_TESTS, getNumSequenceAnnotations());
	cleanupSequenceAnnotations();
	CuAssertIntEquals(tc, 0, getNumSequenceAnnotations());
}

void TestSingleCollection(CuTest* tc) {
	cleanupCollections();
	CuAssertIntEquals(tc, 0, getNumCollections());
	Collection* col = createCollection("one");
	CuAssertIntEquals(tc, 1, getNumCollections());
	CuAssertPtrEquals(tc, col, getNthCollection(0));
	CuAssertIntEquals(tc, 1, isCollectionURI("one"));
	CuAssertIntEquals(tc, 1, isCollectionPtr(col));
	deleteCollection(col);
	CuAssertIntEquals(tc, 0, getNumCollections());
	CuAssertIntEquals(tc, 0, isCollectionURI("one"));
	CuAssertIntEquals(tc, 0, isCollectionPtr(col));
}

void TestNumCollections(CuTest* tc) {
	cleanupCollections();
	char* uri;
	Collection* col;
	int num;
	for (num=0; num<NUM_FAST_TESTS; num++) {
		CuAssertIntEquals(tc, num, getNumCollections());
		uri = randomUniqueURI();
		col = createCollection(uri);
		CuAssertIntEquals(tc, num+1, getNumCollections());
	}
	for (; num>0; num--) {
		CuAssertIntEquals(tc, num, getNumCollections());
		col = getNthCollection(0);
		deleteCollection(col);
		CuAssertIntEquals(tc, num-1, getNumCollections());
	}
	CuAssertIntEquals(tc, 0, getNumCollections());
}

void TestCollectionIndexing(CuTest* tc) {
	cleanupCollections();
	char* uri;
	Collection* col;
	int num;
	for (num=0; num<NUM_SLOW_TESTS; num++) {
		uri = randomUniqueURI();
		// avoid duplicates
		while (isCollectionURI(uri))
			uri = randomUniqueURI();
		col = createCollection(uri);
	}
	int index;
	for (num=NUM_SLOW_TESTS; num>0; num--) {
		CuAssertIntEquals(tc, num, getNumCollections());
		index = randomNumber(num);
		col = getNthCollection(index);
		uri = getCollectionURI(col); // copy uri (because it will be destroyed)
		CuAssertIntEquals(tc, 1, isCollectionPtr(col));
		CuAssertIntEquals(tc, 1, isCollectionURI(uri));
		deleteCollection(col);
		CuAssertIntEquals(tc, num-1, getNumCollections());
		CuAssertIntEquals(tc, 0, isCollectionPtr(col));
		CuAssertIntEquals(tc, 0, isCollectionURI(uri));
	}	
}

void PrintArrayTestInfo() {
	printf("testing arrays\n");
}

CuSuite* ArrayGetSuite() {
	CuSuite* arrayTests = CuSuiteNew();
	SUITE_ADD_TEST(arrayTests, PrintArrayTestInfo);
	SUITE_ADD_TEST(arrayTests, TestSingleDNAComponent);
	SUITE_ADD_TEST(arrayTests, TestNumDNAComponents);
	SUITE_ADD_TEST(arrayTests, TestDNAComponentIndexing);
	SUITE_ADD_TEST(arrayTests, TestSingleAnnotation);
	SUITE_ADD_TEST(arrayTests, TestNumAnnotations);
	SUITE_ADD_TEST(arrayTests, TestAnnotationIndexing);
	SUITE_ADD_TEST(arrayTests, TestCleanupSequenceAnnotations);
	SUITE_ADD_TEST(arrayTests, TestSingleCollection);
	SUITE_ADD_TEST(arrayTests, TestNumCollections);
	SUITE_ADD_TEST(arrayTests, TestCollectionIndexing);
	return arrayTests;
}
