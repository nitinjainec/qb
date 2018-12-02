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
  std::vector<Buffer> data;
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
  Buffer getData () {
    assert (!eod ());
    assert (idx < data.size ());
    return data[idx++];
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
#endif
