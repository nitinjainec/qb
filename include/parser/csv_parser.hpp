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

  RecordPtr parseBufferToRecord (ByteBuffer &buffer) {
    StatRecorder sr ("Parsing record from csv");
    std::vector <std::string> fields;
    for (int idx = buffer.find (',');
	 idx != -1; idx = buffer.find (',')) {
      fields.push_back (std::string (buffer.c_str (), idx));
      buffer.erase (idx+1);
    }
    if (buffer.size () != 0)
      fields.push_back (std::string (buffer.c_str (), buffer.size ()));
    return RecordFactory::create (fields);

    /*
    StatRecorder sr ("Parsing record from csv");
    std::vector <ByteBuffer> fields;
    for (int idx = buffer.find (',');
	 idx != -1; idx = buffer.find (',')) {
      fields.push_back (ByteBuffer (buffer, idx));
      buffer.erase (idx+1);
    }
    if (buffer.size () != 0)
      fields.push_back (buffer);
    return RecordFactory::create (fields);
    */
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
    ByteBuffer buffer = reader->getData ();
    return parseBufferToRecord (buffer);
  }

  bool eor () {
    return reader->eod ();
  }
};

#endif
