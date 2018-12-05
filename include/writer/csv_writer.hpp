#ifndef __CSV_WRITER_HPP__
#define __CSV_WRITER_HPP__
#include <fstream>
#include <string>

#include <interface.hpp>
#include <util/logger.hpp>
#include <statistics/stat_recorder.hpp>

/*
 * CSV writer class to write give data on disk
 * Data is written in chunks to optimize disk i/o.
 * Chunk size written on disk is defined by FILE_BUFFER_SIZE constant.
 */
class CSVWriter : public IWriter {
  std::ofstream _file;
  ByteBuffer _buffer;
  
  void flush () {
    StatRecorder sr ("Writing csv file");
    _file.write (_buffer.c_str (), _buffer.size ());
    _buffer.erase (_buffer.size ());
  }

public:
  CSVWriter (const std::string &filename)
    : _buffer (constants::FILE_BUFFER_SIZE)
  {
    _file.open (filename.c_str (), std::ios::out | std::ios::trunc);
    if (_file.fail ()) {
      throw std::runtime_error ("Can not open file: " + filename);
    }
  }

  /* on receiving notification writes data on disk */
  void notify (const RecordPtr &record) {
    write (record->toString ());
  }

  /* writes given byte buffer on disk */
  void write (const ByteBuffer &buffer) {
    if (_buffer.size () + buffer.size () + 2 > _buffer.capacity ())
      flush ();
    _buffer.append (buffer);
    _buffer.append ("\n");    
  }

  ~CSVWriter () {
    flush ();
    _file.close ();
  }
};

#endif
