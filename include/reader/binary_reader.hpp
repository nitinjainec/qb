#ifndef __BINARY_READER_HPP__
#define __BINARY_READER_HPP__

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <interface.hpp>
#include <statistics/stat_recorder.hpp>
#include <util/byte_buffer.hpp>

const size_t FILE_BUFFER_SIZE = 512 * 1024;

class BinaryReader : public IReader {
  std::ifstream file;
  ByteBuffer buffer;
  size_t count;
  bool data_read;

  void readData () {
    StatRecorder sr ("Reading binary file");
    count = 0;
    if (file.eof ())
      return;

    DLOG("Reading data");
    file.read (buffer.toAppend (FILE_BUFFER_SIZE), FILE_BUFFER_SIZE);
    count = file.gcount ();
    buffer.appended (count);
    data_read = false;
    DLOG("Read " + std::to_string (count) + " bytes");
  }

public:
  BinaryReader (const std::string &filename)
    : file (filename.c_str (), std::ios::in | std::ios::binary)
    , buffer (FILE_BUFFER_SIZE * 2)
  {
    if (file.fail ()) {
      throw std::runtime_error ("Can not open file: " + filename);
    }
    DLOG ("Opened file: " + filename);
    readData ();
  }

  /* Returns chunk of data */
  ByteBuffer getData () {
    assert (!eod ());
    data_read = true;
    ByteBuffer result (buffer);
    buffer.erase (count);
    return result;
  }

  /* Returns true for end of data */ 
  bool eod () {
    if (buffer.size () != 0)
      //if (!data_read)
      return false;
    readData ();
    return buffer.size () == 0;
    //count == 0;
  }

  ~BinaryReader () {
    file.close ();
  }
};

#endif
