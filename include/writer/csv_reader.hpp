#ifndef __CSV_WRITER_HPP__
#define __CSV_WRITER_HPP__
#include <fstream>
#include <string>

#include <interface.hpp>
#include <logger.hpp>
#include <statistics.hpp>


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
      throw std::runtime_error ("Can not open file: " + filename);
    }
    DLOG ("File opened : " + filename);
  }

  void notify (const RecordPtr &record) {
    VLOG ("Notified: " + record->recordTypeName ());
    std::string record_str = record->toString ();
    record_str.append ("\n");
    std::cout << record_str.c_str () << "\n";
    write (record_str.c_str ());
  }

  void write (const ByteBuffer &buffer) {
    StatRecorder sr ("Writing csv");
    VLOG ("Writing data of size: " + std::to_string (buffer.size ()));
    VLOG ("Data: " + std::string (buffer.c_str ()));
    file.write (buffer.c_str (), buffer.size ());
  }

  ~CSVWriter () {
    flush ();
    file.close ();
  }
  
};

#endif
