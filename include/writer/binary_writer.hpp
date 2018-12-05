#ifndef __BINARY_WRITER_HPP__
#define __BINARY_WRITER_HPP__

#include <fstream>
#include <string>

#include <interface.hpp>
#include <util/logger.hpp>
#include <statistics/stat_recorder.hpp>

/*
 * Writes binary data into the file.
 * Data is written in chunks to optimize disk i/o.
 * Chunk size written on disk is defined by FILE_BUFFER_SIZE constant.
 */
class BinaryWriter : public IWriter {
  std::ofstream _file;
  ByteBuffer _buffer;

  void flush () {
    StatRecorder sr ("Writing binary file");
    _file.write (_buffer.c_str (), _buffer.size ());
    _buffer.erase (_buffer.size ());
  }

public:
  BinaryWriter (const std::string &filename)
    : _buffer (2 * constants::FILE_BUFFER_SIZE)
  {
    _file.open (filename.c_str (),
		std::ios::out | std::ios::trunc | std::ios::binary);
    if (_file.fail ()) {
      throw std::runtime_error ("Can not open file: " + filename);
    }
  }

  /* on receiving notifcation writes record on to disk */
  void notify (const RecordPtr &record) {
    write (record->toByteBuffer ());
  }

  /* writes given byte buffer to disk */
  void write (const ByteBuffer &buffer) {
    if (_buffer.size () + buffer.size () > _buffer.capacity ())
      flush ();
    _buffer.append (buffer);
  }

  ~BinaryWriter () {
    flush ();
    _file.close ();
  }
  
};

#endif // __BINARY_WRITER_HPP__
