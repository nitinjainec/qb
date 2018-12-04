#ifndef __CSV_PARSER_HPP__
#define __CSV_PARSER_HPP__

#include <algorithm>
#include <vector>
#include <sstream>

#include <interface.hpp>
#include <record/record_factory.hpp>
#include <statistics/stat_recorder.hpp>
#include <util/logger.hpp>
#include <util/util.hpp>

class CSVParser : public IParser {
  IReaderPtr reader;
  std::string delimeter;

  RecordPtr parseBufferToRecord (const ByteBuffer &buffer) {
    StatRecorder sr ("Parsing record from csv");
    std::vector <std::string> fields;
    std::istringstream s (buffer.c_str ());
    for (std::string field; std::getline (s, field, ','); ) {
      util::trim (field);
      fields.push_back (field);
    }
    return RecordFactory::create (fields);
  }

public:
  CSVParser (const IReaderPtr &reader,
	     const std::string &delimeter = ",")
    : reader (reader)
    , delimeter (delimeter)
  {
    DLOG ("CSVParser constructed");
  }
  
  RecordPtr nextRecord () {
    assert (!eor ());
    const ByteBuffer & buffer = reader->getData ();
    return parseBufferToRecord (buffer);
  }

  bool eor () {
    return reader->eod ();
  }
};

#endif
