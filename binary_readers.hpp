#ifndef __BINARY_READERS_HPP__
#define __BINARY_READERS_HPP__

#include <vector>
#include <string>

#include "interface.hpp"
#include "logger.hpp"
#include "reader.hpp"


class BinaryReaders : public IReader {
  std::vector <IReaderPtr> _readers;
  int _idx;
  IReaderPtr _current_reader;

public:
  BinaryReaders (const std::vector <std::string> filenames)
    : _idx (-1)
  {
    for (int i = 0; i < filenames.size (); ++i) {
      const std::string &filename = filenames [i];
      IReaderPtr reader (new BinaryReader (filename));
      _readers.push_back (reader);
      _idx = 0;
      _current_reader = _readers [_idx];
    }
  }

  /* Returns binary data */
  ByteBuffer getData () {
    assert (!eod ());
    return _current_reader->getData ();
  }

  /* Returns true for end of data */
  bool eod () {
    if (_current_reader && !_current_reader->eod ())
      return false;
    if (++_idx >= _readers.size ())
      return true;
    _current_reader = _readers[_idx];
    return _current_reader->eod ();
  }
  
};
#endif
