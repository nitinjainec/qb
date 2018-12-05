#ifndef __BINARY_READERS_HPP__
#define __BINARY_READERS_HPP__

#include <vector>
#include <string>

#include <interface.hpp>
#include <util/logger.hpp>
#include <reader/binary_reader.hpp>

/*
 * Class to reads data from multiple binary file sequentially.
 */
class BinaryReaders : public IReader {
  std::vector <IReaderPtr> _readers;
  int _idx;
  IReaderPtr _current_reader;

public:
  BinaryReaders (const std::vector <std::string> &filenames)
    : _idx (-1)
  {
    for (int i = 0; i < filenames.size (); ++i) {
      const std::string &filename = filenames [i];
      IReaderPtr reader (new BinaryReader (filename));
      _readers.push_back (reader);
    }
  }

  /* Returns binary data sequentially from each reader */
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
#endif // __BINARY_READERS_HPP__
