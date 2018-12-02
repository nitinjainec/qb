#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include <assert.h>
#include <memory>

typedef std::string Buffer;
const int BUFFER_SIZE = 1024;
const int RECORD_SIZE_BYTES = sizeof (int16_t);

/*
  Reader interface to get data.
  Can be extended to fetch data from file, database, network etc.
*/
class IReader {
public:
  /* returns data buffer */
  virtual Buffer getData () = 0;

  /* returns length of characters read in last call to getData () */
  virtual size_t length () = 0;
  
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
  virtual Buffer toBinaryBuffer () = 0;
  virtual std::string toString () = 0;

  /* Returns size of the record */
  virtual size_t vsize () = 0;

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
  virtual void write (const Buffer &buffer, const size_t size) = 0;
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
  std::string datetime;
  Datetime () : datetime ("YYYY-MM-DD HH:MM:SS.MSS") {}
  Datetime (const std::string &datetime)
    : datetime (datetime)
  {}

  Datetime& operator= (const Buffer &buffer) {
    datetime = std::string (buffer.c_str (), size ());
  }

  std::string toString () {
    return datetime;
  }

  static size_t size () {
    char ch [] = "YYYY-MM-DD HH:MM:SS.MSS";
    return sizeof (ch);
  }
};
#endif
