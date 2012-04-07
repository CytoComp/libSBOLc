import unittest
import sbol
import random
import string

URIS_USED = set()
RANDOM_CHARS = string.ascii_letters
NUM_FAST_TESTS = 10000
NUM_SLOW_TESTS =   100

def random_string(limit=10):
    length = random.randint(0, limit)
    string = ''.join(random.choice(RANDOM_CHARS) for n in range(length))
    return string

def random_uri(limit=10):
    while True:
        uri = random_string()
        global URIS_USED
        if not uri in URIS_USED:
            URIS_USED.add(uri)
            return uri

def random_valid_position(limit=1000):
    return random.randint(0, limit)

def random_invalid_position(limit=1000):
    position = 0
    while position == 0:
        position = -1 * random_valid_position(limit)
    return position

class TestSBOLObject(unittest.TestCase):
    def assertReadOnly(self, obj, attr):
        try:
            obj.__getattribute__(attr)
            obj.__setattr__(attr, None)
            raise AssertionError
        except AttributeError:
            pass

    def assertReadWrite(self, obj, attr, content=None):
        if not content:
            content = random_string()
        self.assertEqual(obj.__getattribute__(attr), None)
        obj.__setattr__(attr, content)
        self.assertEqual(obj.__getattribute__(attr), content)
        obj.__setattr__(attr, None)
        self.assertEqual(obj.__getattribute__(attr), None)

    def setUp(self):
        try:
            self.assertEqual(sbol.libsbol.getNumSBOLObjects(), 0)
        except AssertionError:
            # this test fails, but don't want to mess up the rest
            sbol.libsbol.cleanupSBOLCore()
        self.uris    = []
        self.testees = []
        self.createTestees()
        self.assertEqual(len(self.uris), len(self.testees))
        self.assertEqual(len(self.uris), sbol.libsbol.getNumSBOLObjects())
        self.assertEqual(len(self.uris), len(sbol.ALL_SBOL_OBJECTS.sbol_objects))

    def createTestees(self):
        for obj in (sbol.DNASequence,
                    sbol.SequenceAnnotation,
                    sbol.DNAComponent,
                    sbol.Collection):
            uri = random_uri()
            self.uris.append(uri)
            self.testees.append(obj(uri))

    def testURI(self):
        for n in range( len(self.testees) ):
            testee = self.testees[n]
            uri = self.uris[n]
            self.assertEqual(testee.uri, uri)
            self.assertReadOnly(testee, 'uri')

    def tearDown(self):
        self.assertEqual(len(self.uris), len(sbol.ALL_SBOL_OBJECTS.sbol_objects))
        self.assertEqual(len(self.uris), sbol.libsbol.getNumSBOLObjects())
        del self.testees
        try:
            self.assertEqual(sbol.libsbol.getNumSBOLObjects(), 0)
        except AssertionError:
            # this test failed, but still want to protect the others
            sbol.libsbol.cleanupSBOLCore()
            raise

class TestSBOLCompoundObject(TestSBOLObject):
    def createTestees(self):
        for obj in (sbol.DNAComponent,
                    sbol.Collection):
            uri = random_uri()
            self.uris.append(uri)
            self.testees.append(obj(uri))

    def testDisplayID(self):
        for testee in self.testees:
            self.assertReadWrite(testee, 'display_id')

    def testName(self):
        for testee in self.testees:
            self.assertReadWrite(testee, 'name')

    def testDescription(self):
        for testee in self.testees:
            self.assertReadWrite(testee, 'description')

class TestDNASequence(TestSBOLObject):
    def createTestees(self):
        uri = random_uri()
        self.uris.append(uri)
        self.testees.append( sbol.DNASequence(uri) )

    def testNucleotides(self):
        self.assertReadWrite(self.testees[0], 'nucleotides')

class TestSequenceAnnotation(TestSBOLObject):
    def assertPositionWorking(self, obj, attr):
        self.assertEqual(obj.__getattribute__(attr), None)
        # check that setting valid positions works
        for n in range(NUM_FAST_TESTS):
            position = random_valid_position()
            obj.__setattr__(attr, position)
            self.assertEqual(obj.__getattribute__(attr), position)
        # check that setting invalid positions raises error
        for n in range(NUM_FAST_TESTS):
            obj.__setattr__(attr, None)
            position = random_invalid_position()
            self.assertRaises(sbol.PositionError, obj.__setattr__, attr, position)
        # check that resetting to None works
        obj.__setattr__(attr, 1)
        self.assertEqual(obj.__getattribute__(attr), 1)
        obj.__setattr__(attr, None)
        self.assertEqual(obj.__getattribute__(attr), None)

    def createTestees(self):
        uri = random_uri()
        self.uris.append(uri)
        self.testees.append( sbol.SequenceAnnotation(uri) )

    def testPositions(self):
        self.assertPositionWorking(self.testees[0], 'start')
        self.assertPositionWorking(self.testees[0], 'end')

    def testStrand(self):
        # check that strand is initially forward
        self.assertEquals(self.testees[0].strand, '+')
        # check that it can only be set to valid symbols
        valid_polarities = ('+', '*', '-')
        for symbol in random_string():
            if symbol in valid_polarities:
                self.testees[0].strand = symbol
                self.assertEquals(self.testees[0].strand, symbol)
            else:
                self.assertRaises(sbol.StrandError,
                                  self.testees[0].__setattr__,
                                  'strand',
                                  symbol)

    def testSubcomponent(self):
        self.assertEquals(self.testees[0].subcomponent, None)
        uri = random_uri()
        self.uris.append(uri)
        com = sbol.DNAComponent(uri)
        self.testees[0].subcomponent = com
        self.assertEquals(self.testees[0].subcomponent, com)

    def testPrecedes(self):     pass

class TestDNAComponent(TestSBOLCompoundObject):
    def createTestees(self):
        uri = random_uri()
        self.uris.append(uri)
        self.testees.append( sbol.DNAComponent(uri) )

    def testSequence(self):    pass
    def testAnnotations(self): pass

class TestCollection(TestSBOLCompoundObject):
    def createTestees(self):
        uri = random_uri()
        self.uris.append(uri)
        self.testees.append( sbol.Collection(uri) )

    def testComponents(self):
        col = self.testees[0]
        for n in range(NUM_SLOW_TESTS):
            self.assertEqual(len(col.components), n)
            uri = random_uri()
            self.uris.append(uri)
            com = sbol.DNAComponent(uri)
            self.assertFalse(com in col.components)
            col.components += com
            self.assertTrue(com in col.components)
            self.assertEqual(len(col.components), n+1)

if __name__ == '__main__':
    unittest.main()

