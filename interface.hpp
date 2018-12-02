#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include <memory>

typedef std::string Buffer;
const int BUFFER_SIZE = 1024;

/*
  Reader interface to get data.
  Can be extended to fetch data from file, database, network etc.
*/
class IReader {
public:
  /* returns data buffer */
  virtual Buffer getData () = 0;
  
  /* returns true if end of data is reached */
  virtual bool eod () = 0;
};
typedef std::shared_ptr<IReader> IReaderPtr;

/*
  Record base structure.
  Can be extended to Quote, Trade, Signal records.
*/
struct Record {
  int32_t id;
  virtual Buffer toBinaryBuffer () = 0;
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
  virtual void write (const Buffer &buffer) = 0;
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
  Datetime (const std::string &datetime)
    : datetime (datetime)
  {}

  size_t size () {
    return sizeof (datetime);
  }
};
#endif
