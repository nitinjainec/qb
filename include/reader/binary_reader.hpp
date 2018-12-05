#ifndef __BINARY_READER_HPP__
#define __BINARY_READER_HPP__

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <interface.hpp>
#include <statistics/stat_recorder.hpp>
#include <util/byte_buffer.hpp>

/*
 * Reads file in binary format and returns data of size 
 * FILE_BUFFER_SIZE bytes
 */
class BinaryReader : public IReader {
  std::ifstream file;
  ByteBuffer buffer;

  void readData () {
    StatRecorder sr ("Reading binary file");
    if (file.eof ())
      return;
    file.read (buffer.toAppend (constants::FILE_BUFFER_SIZE),
	       constants::FILE_BUFFER_SIZE);
    buffer.appended (file.gcount ());
    DLOG("Read " + std::to_string (buffer.size ()) + " bytes");
  }

public:
  BinaryReader (const std::string &filename)
    : file (filename.c_str (), std::ios::in | std::ios::binary)
    , buffer (2 * constants::FILE_BUFFER_SIZE)
  {
    if (file.fail ()) {
      throw std::runtime_error ("Can not open file: " + filename);
    }
    readData ();
  }

  /* Returns chunk of data of max size FILE_BUFFER_SIZE */
  ByteBuffer getData () {
    assert (!eod ());
    ByteBuffer result (buffer);
    buffer.erase (buffer.size ());
    return result;
  }

  /* Returns true for end of data */ 
  bool eod () {
    if (buffer.size () == 0)
      readData ();    
    return buffer.size () == 0;
  }

  ~BinaryReader () {
    file.close ();
  }
};

#endif
