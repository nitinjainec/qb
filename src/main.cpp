#include <iostream>

#include <reader/binary_readers.hpp>
#include <reader/csv_reader.hpp>
#include <record_factory/binary_record_factory.hpp>
#include <record_factory/csv_record_factory.hpp>
#include <parser/binary_parser.hpp>
#include <parser/csv_parser.hpp>
#include <processor/processor.hpp>
#include <statistics/stat_recorder.hpp>
#include <util/logger.hpp>
#include <writer/binary_writer.hpp>
#include <writer/binary_writers.hpp>
#include <writer/csv_writer.hpp>


const std::string INPUT_FILE = "../data/qb.csv";
const std::string OUTPUT_DIR = "../output/";
const std::string FILE_PREFIX= "Symbol-";

void csvToBinary () {
  IReaderPtr reader (new CSVReader (INPUT_FILE));
  CSVRecordFactoryPtr factory (new CSVRecordFactory ());
  IParserPtr parser (new CSVParser (reader, factory));
  IProcessorPtr processor (new Processor (parser));
  IWriterPtr writer (new BinaryWriters (OUTPUT_DIR + FILE_PREFIX));

  processor->registerWriter (writer);
  processor->process ();
}

void binaryToCSV () {
  std::vector <std::string> filenames = util::linux::listDirectory (OUTPUT_DIR, FILE_PREFIX);
  IReaderPtr reader (new BinaryReaders (filenames));
  BinaryRecordFactoryPtr factory (new BinaryRecordFactory ());  
  IParserPtr parser (new BinaryParser (reader, factory));
  IProcessorPtr processor (new Processor (parser));
  IWriterPtr writer (new CSVWriter ("output.csv"));
  processor->registerWriter (writer);
  processor->process ();
}

int main () {
  Logger::create ();
  try {
    StatRecorder sr ("Main");
    csvToBinary ();
    binaryToCSV ();
  }
  catch (const std::exception& ex) {
    std::cout << "Exception: " << ex.what () << "\n";
  }
  StatRecorder::display (std::cout);
}
