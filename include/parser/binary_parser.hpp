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

/*
 * Parses binary data received from binary reader to Record
 */
class BinaryParser : public IParser {
  IReaderPtr _reader;
  ByteBuffer _buffer;

  RecordPtr parseBufferToRecord () {
    StatRecorder sr ("Parsing record from binary");
    RecordPtr record (RecordFactory::create (_buffer));
    _buffer.erase (record->size ());
    return record;
  }

  bool canParseBufferToRecord () {
    return RecordFactory::canCreate (_buffer);
  }

  void getData () {
    const ByteBuffer & buff = _reader->getData ();
    _buffer.append (buff);
  }

public:
  BinaryParser (const IReaderPtr &reader)
    : _reader (reader)
    , _buffer (2 * constants::FILE_BUFFER_SIZE)
  { }

  /* Parses binary data and returns next Record */
  RecordPtr nextRecord () {
    StatRecorder sr ("Parse binary data to Record");
    assert (!eor ());
    while (!canParseBufferToRecord () && !eor ())
      getData ();
    if (canParseBufferToRecord ())
      return parseBufferToRecord ();
    throw std::runtime_error ("Unable to parse binary buffer to record.");
  }

  /* Returns true once end of Record is reached */
  bool eor () {
    return !canParseBufferToRecord () && _reader->eod ();
  }
};
#endif // __PARSER_HPP__
