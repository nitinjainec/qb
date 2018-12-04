#include <iostream>

#include "binary_readers.hpp"
#include "binary_writers.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "processor.hpp"
#include "reader.hpp"
#include "statistics.hpp"
#include "writer.hpp"

void csvToBinary () {
  IReaderPtr reader (new CSVReader ("qb.csv"));
  IParserPtr parser (new CSVParser (reader));
  IProcessorPtr processor (new CSVToBinaryProcessor (parser));
  //IWriterPtr writer (new BinaryWriter ("filename.txt"));
  IWriterPtr writer (new BinaryWriters ("Symbol-"));

  processor->registerWriter (writer);
  processor->process ();
}

void binaryToCSV () {
//IReaderPtr reader (new BinaryReader ("filename.txt"));
std::vector <std::string> filenames;
filenames.push_back ("Symbol-ZBM3");
filenames.push_back ("Symbol-ZNM3");

IReaderPtr reader (new BinaryReaders (filenames));

  IParserPtr parser (new BinaryParser (reader));
  IProcessorPtr processor (new BinaryToCSVProcessor (parser));
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
