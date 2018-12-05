#include <assert.h>
#include <iostream>
#include <string.h>

#include <parser/binary_parser.hpp>
#include <parser/csv_parser.hpp>
#include <processor/processor.hpp>
#include <reader/binary_readers.hpp>
#include <reader/csv_reader.hpp>
#include <writer/binary_writers.hpp>
#include <writer/csv_writer.hpp>

#include "unit_test.hpp"

class EndToEndTest : public UnitTest {
  std::string _prefix;

  void csvToBinary () {
    IReaderPtr reader (new CSVReader ("../../test/data/qb.csv"));
    IParserPtr parser (new CSVParser (reader));
    IProcessorPtr processor (new Processor (parser));
    IWriterPtr writer (new BinaryWriters (_prefix));
    processor->registerWriter (writer);
    processor->process ();
  }

  void binaryToCSV () {
    std::vector <std::string> filenames;
    filenames.push_back (_prefix + "ZBM3");
    filenames.push_back (_prefix + "ZNM3");

    IReaderPtr reader (new BinaryReaders (filenames));
    IParserPtr parser (new BinaryParser (reader));
    IProcessorPtr processor (new Processor (parser));
    IWriterPtr writer (new CSVWriter ("output.csv"));
    processor->registerWriter (writer);
    processor->process ();
  }

  void verifyOutput () {
    IReaderPtr e_reader (new BinaryReader ("../../test/data/expected_output.csv"));
    IReaderPtr o_reader (new BinaryReader ("output.csv"));
    ByteBuffer expected (1024 * 1024);
    ByteBuffer output (1024 * 1024);

    while (!e_reader->eod ()) {
      expected.append (e_reader->getData ());
    }
    while (!o_reader->eod ()) {
      output.append (o_reader->getData ());
    }

    assert (expected.size () == output.size ());
    assert (memcmp (expected.c_str (), output.c_str (), expected.size ()) == 0);
  }

  void endToEndTest () {
    csvToBinary ();
    binaryToCSV ();
    verifyOutput ();
    ++_test_passed;
  }

public:
  EndToEndTest ()
    : _prefix ("Symbol-" + util::randomString (8))
  {}

  void operator () () {
    endToEndTest ();
    std::cout << "End to end: " << _test_passed << " test passed" << std::endl;
  }
};
