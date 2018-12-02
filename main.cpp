#include <iostream>

#include "logger.hpp"
#include "parser.hpp"
#include "processor.hpp"
#include "reader.hpp"
#include "writer.hpp"

void csvToBinary () {
  IReaderPtr reader (new CSVReader ("qb.csv"));
  IParserPtr parser (new CSVParser (reader));
  IProcessorPtr processor (new CSVToBinaryProcessor (parser));
  IWriterPtr writer (new BinaryWriter ("filename.txt"));

  processor->registerWriter (writer);
  processor->process ();
}

void binaryToCSV () {
  IReaderPtr reader (new BinaryReader ("filename.txt"));
  IParserPtr parser (new BinaryParser (reader));
  IProcessorPtr processor (new BinaryToCSVProcessor (parser));
  IWriterPtr writer (new CSVWriter ("output.csv"));
  processor->registerWriter (writer);
  processor->process ();
}

int main () {
  Logger::create ();

  try {
    csvToBinary ();
    binaryToCSV ();
  }
  catch (const std::exception& ex) {
    std::cout << "Exception: " << ex.what () << "\n";
  }
}
