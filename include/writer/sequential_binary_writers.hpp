#ifndef __SEQUENTIAL_BINARY_WRITERS__
#define __SEQUENTIAL_BINARY_WRITERS__

#include <assert.h>
#include <map>

#include <interface.hpp>
#include <util/logger.hpp>
#include <util/serializer.hpp>
#include <writer/binary_writer.hpp>

/*
  Class to write each symbol record in its own binary file with sequence number
*/
class SequentialBinaryWriters : public IWriter {
  // TODO: string as key is in-efficient
  // Evaluate performance impact and replace it by string hash
  std::map <std::string, IWriterPtr> _symbol_to_writer;
  IWriterPtr _writer;
  std::string _prefix;
  uint32_t _count;

  void setWriterForRecord (const RecordPtr &record) {
    if (_symbol_to_writer.find (record->symbolName ())
	== _symbol_to_writer.end ()) {
      _writer.reset (new BinaryWriter (_prefix + record->symbolName ()));
      _symbol_to_writer[record->symbolName ()] = _writer;
    }
    else
      _writer = _symbol_to_writer[record->symbolName ()];    
  }
public:

  SequentialBinaryWriters (const std::string &prefix)
    : _prefix (prefix)
    , _count (0)
  {}

  void notify (const RecordPtr &record) {
    setWriterForRecord (record);
    Serializer sr;
    sr << ++_count << record->size () << *record;
    write (sr.toByteBuffer ());
  }

  void write (const ByteBuffer &buffer) {
    assert (_writer);
    _writer->write (buffer);
    _writer.reset ();
  }
};

#endif
