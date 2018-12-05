#ifndef __CSV_READER_HPP__
#define __CSV_READER_HPP__

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <interface.hpp>
#include <statistics/stat_recorder.hpp>

/*
 * Reads csv file and returns data line by line
 */
class CSVReader : public IReader {
  std::ifstream file;
  ByteBuffer buffer;

  void readData () {
    StatRecorder sr ("Reading csv file");
    if (file.eof ())
      return;

    file.read (buffer.toAppend (constants::FILE_BUFFER_SIZE),
	       constants::FILE_BUFFER_SIZE);
    buffer.appended (file.gcount ());
    DLOG("Read " + std::to_string (buffer.size ()) + " bytes");
  }

public:
  CSVReader (const std::string &filename)
    : file (filename.c_str ())
    , buffer (2 * constants::FILE_BUFFER_SIZE)
  {
    if (file.fail ())
      throw std::runtime_error ("Can not open file: " + filename);
    readData ();
  }
  
  /* Returns single line as data */
  ByteBuffer getData () {
    assert (!eod ());
    int idx = buffer.newLine ();
    if (idx == -1) {
      if (file.eof ()) {
	ByteBuffer result (buffer);
	buffer.erase (buffer.size ());
	return result;
      }
      readData ();
      return getData ();
    }
    else {
      ByteBuffer result (buffer, idx);
      buffer.erase (idx + 1);
      return result;
    }
  }

  /* Returns true for end of data */
  bool eod () {
    if (buffer.size () == 0)
      readData ();
    return buffer.size () == 0;
  }

  ~CSVReader () {
    file.close ();
  }
};

#endif
