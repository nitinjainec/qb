#ifndef __CSV_READER_HPP__
#define __CSV_READER_HPP__

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <interface.hpp>
#include <statistics/stat_recorder.hpp>

const size_t CSV_FILE_BUFFER_SIZE = 1024 * 1024;

class CSVReader : public IReader {
  std::ifstream file;
  ByteBuffer buffer;

  void readData () {
    StatRecorder sr ("Reading csv file");
    DLOG("Reading csv");    
    if (file.eof ())
      return;

    file.read (buffer.toAppend (CSV_FILE_BUFFER_SIZE), CSV_FILE_BUFFER_SIZE);
    buffer.appended (file.gcount ());
    DLOG("Read " + std::to_string (buffer.size ()) + " bytes");
  }

public:
  CSVReader (const std::string &filename)
    : file (filename.c_str ())
    , buffer (CSV_FILE_BUFFER_SIZE)
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
    int idx = buffer.newLine ();
    if (idx != -1) {
      ByteBuffer result (buffer, idx - 1);
      buffer.erase (idx + 1);
      return result;
    }
    else {
      if (!file.eof ()) {
	readData ();
	return getData ();
      }
      ByteBuffer result (buffer);
      buffer.erase (buffer.size ());
      return result;
    }
  }

  /* Returns true for end of data */
  bool eod () {
    if (buffer.size () != 0)
      return false;
    readData ();
    if (buffer.size () == 0)
      assert (file.eof ());
    return buffer.size () == 0;
  }

  ~CSVReader () {
    file.close ();
  }
};

#endif
