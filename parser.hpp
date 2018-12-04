#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <algorithm>
#include <vector>
#include <sstream>

#include "interface.hpp"
#include "logger.hpp"
#include "record_factory.hpp"
#include "statistics.hpp"
#include "util.hpp"

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

class BinaryParser : public IParser {
  IReaderPtr reader;
  ByteBuffer buffer;

  RecordPtr parseBufferToRecord () {
    StatRecorder sr ("Parsing record from binary");
    RecordPtr record (RecordFactory::create (buffer));
    buffer.erase (record->vsize ());
    return record;
  }

  bool canParseBufferToRecord () {
    return RecordFactory::canCreate (buffer);
  }

  void getData () {
    const ByteBuffer & buff = reader->getData ();
    buffer.append (buff);
  }

public:
  BinaryParser (const IReaderPtr &reader)
    : reader (reader)
  {
    DLOG ("BinaryParser constructed");
    buffer.reserve (BUFFER_SIZE * 2);
  }
  
  RecordPtr nextRecord () {
    assert (!eor ());
    while (!canParseBufferToRecord ()
	   && !eor ())
      getData ();
    if (canParseBufferToRecord ())
      return parseBufferToRecord ();
    throw std::runtime_error ("Unable to parse binary buffer to record.");
  }

  bool eor () {
    return !canParseBufferToRecord () && reader->eod ();
  }
};
#endif
