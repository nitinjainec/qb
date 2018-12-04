#include <iostream>

#include <reader/binary_readers.hpp>
#include <reader/csv_reader.hpp>
#include <parser/binary_parser.hpp>
#include <parser/csv_parser.hpp>
#include <processor/processor.hpp>
#include <statistics/stat_recorder.hpp>
#include <util/logger.hpp>
#include <writer/binary_writer.hpp>
#include <writer/binary_writers.hpp>
#include <writer/csv_writer.hpp>

void csvToBinary () {
  IReaderPtr reader (new CSVReader ("../data/qb.csv"));
  IParserPtr parser (new CSVParser (reader));
  IProcessorPtr processor (new Processor (parser));
  IWriterPtr writer (new BinaryWriters ("../output/Symbol-"));

  processor->registerWriter (writer);
  processor->process ();
}

void binaryToCSV () {
  //IReaderPtr reader (new BinaryReader ("filename.txt"));
  std::vector <std::string> filenames;
  filenames.push_back ("../output/Symbol-ZBM3");
  filenames.push_back ("../output/Symbol-ZNM3");

  IReaderPtr reader (new BinaryReaders (filenames));

  IParserPtr parser (new BinaryParser (reader));
  IProcessorPtr processor (new Processor (parser));
  IWriterPtr writer (new CSVWriter ("output.csv"));
  processor->registerWriter (writer);
  processor->process ();
}

void test () {
  IReaderPtr reader (new CSVReader ("qb.csv"));
  IParserPtr parser (new CSVParser (reader));
  RecordPtr record = parser->nextRecord ();
  const ByteBuffer &buffer = record->toByteBuffer ();
  const char *ch = buffer.c_str ();

  //assert (*(reinterpret_cast<const RecordType *>(ch)) == QUOTE);
}

int main () {
  Logger::create ();

#if 0
  test ();
  return 0;
#endif
  try {
    csvToBinary ();
    binaryToCSV ();

    std::cout << "-------------------\n";
    StatRecorder::display (std::cout);
  }
  catch (const std::exception& ex) {
    std::cout << "Exception: " << ex.what () << "\n";
  }
}
