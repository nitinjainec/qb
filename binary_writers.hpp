#ifndef __BINARY_WRITERS__
#define __BINARY_WRITERS__

#include <assert.h>
#include <map>

#include "interface.hpp"
//#include "logger.hpp"
#include "writer.hpp"

/*
  Class to write each symbol record in its own binary file
*/
class BinaryWriters : public IWriter {
  // TODO: string as key is in-efficient
  // Evaluate performance impact and replace it by string hash
  std::map <std::string, IWriterPtr> _symbol_to_writer;
  IWriterPtr _writer;
  std::string _prefix;

public:

  BinaryWriters (const std::string &prefix)
    : _prefix (prefix)
  {}

  void notify (const RecordPtr &record) {
    if (_symbol_to_writer.find (record->symbolName ()) == _symbol_to_writer.end ()) {
      _writer.reset (new BinaryWriter (_prefix + record->symbolName ()));
      _symbol_to_writer[record->symbolName ()] = _writer;
    }
    else
      _writer = _symbol_to_writer[record->symbolName ()];
    write (record->toByteBuffer ());
  }

  void write (const ByteBuffer &buffer) {
    assert (_writer);
    _writer->write (buffer);
    _writer.reset ();
  }
};


#endif
