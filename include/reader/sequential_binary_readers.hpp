#ifndef __SEQUENTIAL_BINARY_READERS_HPP__
#define __SEQUENTIAL_BINARY_READERS_HPP__

#include <vector>
#include <string>

#include <interface.hpp>
#include <util/logger.hpp>
#include <util/serializer.hpp>
#include <reader/binary_reader.hpp>

/*
 * Class to reads data from multiple binary file sequentially.
 */
class SequentialBinaryReaders : public IReader {
  typedef std::pair <IReaderPtr, ByteBuffer> RValue;
  typedef std::map <uint32_t, RValue> RMap;
  RMap _readers;
  uint32_t _count;

  void addToMap (RValue &v) {
    addToMap (v.first, v.second);
  }

  void addToMap (const IReaderPtr &reader, ByteBuffer &buffer) {
    if (buffer.size () < sizeof (uint32_t) + sizeof (size_t)) {
      if (reader->eod ())
	assert (buffer.size () == 0);
      else
	buffer.append (reader->getData ());
    }
    Serializer sr (buffer);
    uint32_t id;
    sr >> id;
    _readers[id] = RValue (reader, sr.toByteBuffer ());
  }

  ByteBuffer getRecord () {
    assert (!_readers.empty ());
    RValue &v =  _readers.begin ()->second;
    Serializer sr (v.second);
    size_t size;
    sr >> size;
    size_t rem_size = size > sr.toByteBuffer ().size ()
      ? size - sr.toByteBuffer ().size () : 0;
    size -= rem_size;

    v.second = sr.toByteBuffer ();
    ByteBuffer result (sr.toByteBuffer (), size);
    v.second.erase (size);
    if (rem_size != 0) {
      v.second = v.first->getData ();
      result.append (v.second.c_str (), rem_size);
      v.second.erase (rem_size);
    }
    return result;
  }

public:
  SequentialBinaryReaders (const std::vector <std::string> &filenames)
    : _count (0)
  {
    for (int i = 0; i < filenames.size (); ++i) {
      const std::string &filename = filenames [i];
      IReaderPtr reader (new BinaryReader (filename));
      ByteBuffer buffer = reader->getData ();
      addToMap (reader, buffer);
    }
  }

  /* Returns binary data sequentially from each reader */
  ByteBuffer getData () {
    assert (!eod ());
    assert (_readers.begin ()->first == ++_count);

    const ByteBuffer buffer = getRecord ();
    addToMap (_readers.begin ()->second);
    _readers.erase (_readers.begin ());
    return buffer;
  }

  /* Returns true for end of data */
  bool eod () {
    return _readers.empty ();
  }
};

#endif // __SEQUENTIAL_BINARY_READERS_HPP__
