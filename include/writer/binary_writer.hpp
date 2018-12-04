#ifndef __BINARY_WRITER_HPP__
#define __BINARY_WRITER_HPP__

#include <fstream>
#include <string>

#include <interface.hpp>
#include <util/logger.hpp>
#include <statistics/stat_recorder.hpp>

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
      throw std::runtime_error ("Can not open file: " + filename);
    }
    DLOG ("File opened : " + filename);
  }

  void notify (const RecordPtr &record) {
    VLOG ("Got notification");
    write (record->toByteBuffer ());
  }

  // TODO: Optimize it by writing it to local buffer and flush on disk in chunks
  void write (const ByteBuffer &buffer) {
    StatRecorder sr ("Writing binary data");
    VLOG ("Writing data to file of size: " + std::to_string (buffer.size ()));
    file.write (buffer.c_str (), buffer.size ());
    VLOG ("Data written to file");
  }

  ~BinaryWriter () {
    flush ();
    file.close ();
  }
  
};

#endif
