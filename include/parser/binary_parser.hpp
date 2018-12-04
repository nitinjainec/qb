#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <algorithm>
#include <vector>
#include <sstream>

#include <interface.hpp>
#include <record/record_factory.hpp>
#include <statistics/stat_recorder.hpp>
#include <util/logger.hpp>
#include <util/util.hpp>

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
