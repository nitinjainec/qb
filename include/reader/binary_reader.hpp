#ifndef __BINARY_READER_HPP__
#define __BINARY_READER_HPP__

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <interface.hpp>
#include <statistics/stat_recorder.hpp>

class BinaryReader : public IReader {
  std::ifstream file;
  char buffer [BUFFER_SIZE+1];
  int32_t count;
  bool data_read;

  void readData () {
    StatRecorder sr ("Reading binary data");
    count = 0;
    if (file.eof ())
      return;

    DLOG("Reading data");
    file.read (buffer, BUFFER_SIZE);
    count = file.gcount ();

    data_read = false;
    DLOG("Read " + std::to_string (count) + " bytes");
  }

public:
  BinaryReader (const std::string &filename)
    : file (filename.c_str (), std::ios::in | std::ios::binary)
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
    return ByteBuffer (buffer, count);
  }

  /* Returns lenght of characters read */
  size_t length () {
    return count;
  }
  
  /* Returns true for end of data */ 
  bool eod () {
    if (!data_read)
      return false;
    readData ();
    return count == 0;
  }

  ~BinaryReader () {
    file.close ();
  }
};

#endif
