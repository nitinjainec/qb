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
    VLOG ("Got notification");
    write (record->toBinaryBuffer (), record->vsize ());
  }

  // TODO: Optimize it by writing it to local buffer and flush on disk in chunks
  void write (const Buffer &buffer, const size_t size) {
    VLOG ("Writing data to file of size: " + std::to_string (size));
    file.write (buffer.c_str (), size);
    VLOG ("Data written to file");
  }

  ~BinaryWriter () {
    flush ();
    file.close ();
  }
  
};


class CSVWriter : public IWriter {
  std::ofstream file;
  char buffer [BUFFER_SIZE];
  int32_t idx;
  
  void flush () {
    file.flush ();
  }

public:
  CSVWriter (const std::string &filename)
    : idx (0)
  {
    file.open (filename.c_str (), std::ios::out | std::ios::trunc);
    if (file.fail ()) {
      throw std::runtime_error ((boost::format
				 ("Can not open file: %1%") % filename).str ());
    }
    DLOG ("File opened : " + filename);
  }

  void notify (const RecordPtr &record) {
    std::cout << "Notified \n";
    write (record->toString (), record->vsize ());
  }

  void write (const Buffer &buffer, const size_t size) {
    std::cout << "Writing: " << buffer << "\n";
    file.write (buffer.c_str (), size);
  }

  ~CSVWriter () {
    flush ();
    file.close ();
  }
  
};

#endif
