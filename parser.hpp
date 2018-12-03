#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <algorithm>
#include <vector>
#include <sstream>

#include "interface.hpp"
#include "logger.hpp"
#include "record_factory.hpp"
#include "util.hpp"

class CSVParser : public IParser {
  IReaderPtr reader;
  std::string delimeter;

  RecordPtr parseBufferToRecord (const ByteBuffer &buffer) {
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
  size_t length;

  RecordPtr parseBufferToRecord () {
    RecordPtr record = RecordFactory::create (buffer, length);
    buffer.erase (record->vsize ());
    length -= record->vsize ();
    VLOG (record->recordTypeName ());
    VLOG (std::to_string (record->vsize ()));
    return record;
  }

  bool canParseBufferToRecord () {
    return RecordFactory::canCreate (buffer, length);
  }

  void getData () {
    const ByteBuffer & buff = reader->getData ();
    length += reader->length ();
    buffer.append (buff);
  }

public:
  BinaryParser (const IReaderPtr &reader)
    : reader (reader)
    , length (0)
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
    return reader->eod ();
  }
};
#endif
