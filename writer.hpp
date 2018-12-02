#ifndef __WRITER_HPP__
#define __WRITER_HPP__

#include <fstream>
#include <string>

#include "interface.hpp"

class BinaryWriter : public IWriter {
  std::ofstream file;
  char buffer [BUFFER_SIZE];
  int32_t idx;

  void flush () {
    file.flush ();
  }

public:
  BinaryWriter (const std::string &filename)
    : idx (0)
  {
    file.open (filename.c_str (), std::ios::out | std::ios::trunc | std::ios::binary);
    if (file.fail ()) {
      throw std::runtime_error ((boost::format
				 ("Can not open file: %1%") % filename).str ());
    }
    DLOG ("File opened : " + filename);
  }

  void notify (const RecordPtr &record) {
    write (record->toBinaryBuffer ());
  }

  // TODO: Optimize it by writing it to local buffer and flush on disk in chunks
  void write (const Buffer &buffer) {
    file.write (buffer.c_str (), buffer.size ());
  }

  ~BinaryWriter () {
    flush ();
    file.close ();
  }
  
};

/*
class CSVWriter : public IWriter {
  std::string delimeter;
  std::ofstream file;
public:
  CSVWriter (const std::string &filename,
	     const std::string &delimeter = ",")
    : delimeter (delimeter)
  {

    file.open (filename.c_str (), std::ios::out | std::ios::trunc);
    if (file.fail ()) {
      throw std::runtime_error ((boost::format
				 ("Can not open file: %1%") % filename).str ());
    }
    std::for_each (data.begin (), data.end (),
		   [this] (auto input) { this->writeRow (input);});    
  }

  void notify (const Record &record) {
    
  }
  
  void write () {
  }
  
};
*/

#endif
