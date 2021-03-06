#include <util/logger.hpp>
#include <parser/csv_parser.hpp>
#include <processor/processor.hpp>
#include <reader/csv_reader.hpp>
#include <record_factory/binary_record_factory.hpp>
#include <record_factory/csv_record_factory.hpp>

#include "unit_test.hpp"

class MockReader : public IReader {
  bool _eod;
public:

  MockReader () : _eod (false) { }
  ByteBuffer getData () {
    _eod = true;
    return "2013-04-12 17:00:00.006,ZNM3,132.9531,132.9688,697,3493";
  }
  bool eod () { return _eod; }
};

class QuoteTest : public UnitTest {

  void testSerializeAndDeSerialize () {
    IReaderPtr reader (new MockReader ());
    CSVRecordFactoryPtr factory (new CSVRecordFactory ());
    IParserPtr parser (new CSVParser (reader, factory));
    
    assert (!parser->eor ());
    RecordPtr record_a = parser->nextRecord ();
    assert (parser->eor ());

    ByteBuffer buffer = record_a->toByteBuffer ();

    BinaryRecordFactory bfactory;
    assert (bfactory.canCreateRecord (buffer));
    RecordPtr record_b = bfactory.createRecord (buffer);

    assert (record_a->toString () == record_b->toString ());
    ++_test_passed;
  }

public:
  void operator () () {
    testSerializeAndDeSerialize ();
    std::cout << "Quote: " << _test_passed << " test passed" << std::endl;
  }
};
