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
  IWriterPtr writer(new BinaryWriter ("filename.txt"));

  processor->registerWriter (writer);
  try {
    processor->process ();
  }
  catch (const std::exception& ex) {
    std::cout << "Exception: " << ex.what () << "\n";
  }
}

int main () {
  Logger::create ();

  csvToBinary ();

}
