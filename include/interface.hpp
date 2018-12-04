#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include <assert.h>
#include <memory>

#include <util/byte_buffer.hpp>


const int BUFFER_SIZE = 1024;
const int RECORD_SIZE_BYTES = sizeof (int16_t);

/*
  Reader interface to get data.
  Can be extended to fetch data from file, database, network etc.
*/
class IReader {
public:
  /* returns data buffer */
  virtual ByteBuffer getData () = 0;

  /* returns true if end of data is reached */
  virtual bool eod () = 0;
};
typedef std::shared_ptr<IReader> IReaderPtr;

/* Record Types */
enum RecordType {
  QUOTE = 1,
  TRADE,
  SIGNAL  
};

/*
  Record base structure.
  Can be extended to Quote, Trade, Signal records.
*/

struct Record {
  int32_t id;
  virtual ByteBuffer toByteBuffer () = 0;
  virtual std::string toString () = 0;

  /* Returns record symbol */
  virtual std::string symbolName () = 0;

  /* Returns size of the record */
  virtual size_t vsize () = 0;

  /* Returns readable name of record type */
  virtual std::string recordTypeName () = 0;

  /* Returns record type QUOTE, TRADE, SIGNAL */
  static RecordType recordType () { assert (false); }
};
typedef std::shared_ptr<Record> RecordPtr;

/*
  Parser interface to parse data read by Reader.
*/
class IParser {
public:
  /* returns single record */
  virtual RecordPtr nextRecord () = 0;
  
  /* returns true if end of record is reached */
  virtual bool eor () = 0;
};
typedef std::shared_ptr<IParser> IParserPtr;

/* Forward declaration */
class IProcessor;
typedef std::shared_ptr<IProcessor> IProcessorPtr;

/*
  Writer interface to write the data.
  Can be extended to write the output to a file, database, network etc.
*/
class IWriter {
public:

  /* Notify writer to persist the record */
  virtual void notify (const RecordPtr &record) = 0;

  /* writes the given buffer */
  virtual void write (const ByteBuffer &buffer) = 0;
};
typedef std::shared_ptr<IWriter> IWriterPtr;

/*
  Processor interface to process data and notify registered writers.
*/
class IProcessor {
public:
  /* Register writers to get update once record is processed */
  virtual void registerWriter (const IWriterPtr &writer) = 0;

  /* process the data */
  virtual void process () = 0;
};

struct Datetime {
  char _datetime[24];
  Datetime () {
    memset (_datetime, 'a', size () - 1);
    _datetime [size () - 1] = '\0';
  }
  Datetime (const std::string &datetime)
  {
    assert (datetime.size () == size () - 1);
    memcpy (_datetime, datetime.c_str (), datetime.size ());
    _datetime [size () - 1] = '\0';
  }

  Datetime (const char *ch) {
    memcpy (_datetime, ch, size () - 1);
    _datetime [size () - 1] = '\0';
  }

  Datetime& operator= (const char *ch) {
    memcpy (_datetime, ch, size () - 1);
    _datetime [size () - 1] = '\0';
    return *this;
  }

  std::string toString () {
    return std::string (_datetime);
  }

  ByteBuffer toByteBuffer () {
    return ByteBuffer (_datetime, size ());
  }
  
  static size_t size () {
    return sizeof (_datetime);
  }
};

/*
  Stat interface to record statistics
*/
class IStat {
public:
  virtual void start (const std::string &key) = 0;
  virtual void end (const std::string &key) = 0;
  virtual std::string toString () = 0;
};

typedef std::shared_ptr<IStat> IStatPtr;
#endif
