#ifndef __READER_HPP__
#define __READER_HPP__

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "interface.hpp"

class CSVReader : public IReader {
  char delimeter;
  int32_t idx;
  std::vector<ByteBuffer> data;
  std::ifstream file;

  void readData () {
    DLOG("Reading data");
    data.clear ();
    idx = 0;
    for (std::string line; std::getline (file, line)
	   && data.size () < BUFFER_SIZE;) {
      data.push_back (line);
    }
    DLOG("Read " + std::to_string (data.size ()) + " lines");
  }

public:
  CSVReader (const std::string &filename,
	     const char &delimeter = ',')
    : delimeter (delimeter)
    , file (filename.c_str ())
  {
    if (file.fail ()) {
      throw std::runtime_error ("Can not open file: " + filename);
    }
    DLOG ("Opened file: " + filename);
    readData ();
  }
  
  /* Returns single line as data */
  ByteBuffer getData () {
    assert (!eod ());
    assert (idx < data.size ());
    return data[idx++];
  }

  /* Returns length of characters */
  size_t length () {
    return data[idx-1].size ();
  }

  /* Returns true for end of data */
  bool eod () {
    if (idx < data.size ())
      return false;
    readData ();
    return idx >= data.size ();
  }

  ~CSVReader () {
    file.close ();
  }
};

class BinaryReader : public IReader {
  std::ifstream file;
  char buffer [BUFFER_SIZE+1];
  int32_t count;
  bool data_read;

  void readData () {
    count = 0;
    if (file.eof ())
      return;

    DLOG("Reading data");
    file.read (buffer, BUFFER_SIZE);
    count = file.gcount ();
    buffer [count] = '\0';
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
    return buffer;
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
