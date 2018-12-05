#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include <assert.h>
#include <memory>

#include <util/byte_buffer.hpp>

namespace constants {
  const int BUFFER_SIZE = 1024;
  const size_t FILE_BUFFER_SIZE = 1024 * 1024;
}

/*
  Reader interface to get data.
  Can be extended to fetch data from file, database, network etc.
*/
class IReader {
public:
  /* returns data as ByteBuffer */
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
  /* returns record as ByteBuffer */
  virtual ByteBuffer toByteBuffer () = 0;

  /* returns record as cs string */
  virtual std::string toString () = 0;

  /* Returns record type QUOTE, TRADE, SIGNAL */
  static RecordType recordType () { assert (false); }

  /* returns readable name of record type */
  virtual std::string recordTypeName () = 0;

  /* returns record symbol name */
  virtual std::string symbolName () = 0;

  /* returns size of the record */
  virtual size_t size () = 0;
};
typedef std::shared_ptr<Record> RecordPtr;

/*
  Parser interface to parse data read by IReader.
*/
class IParser {
public:
  /* returns single record */
  virtual RecordPtr nextRecord () = 0;
  
  /* returns true if end of record is reached */
  virtual bool eor () = 0;
};
typedef std::shared_ptr<IParser> IParserPtr;

/*
  Writer interface to write the data.
  Can be extended to write the output to a file, database, network etc.
*/
class IWriter {
public:

  /* interface for receiving notification to persist the record */
  virtual void notify (const RecordPtr &record) = 0;

  /* writes the given ByteBuffer */
  virtual void write (const ByteBuffer &buffer) = 0;
};
typedef std::shared_ptr<IWriter> IWriterPtr;

/*
  Processor interface to process data and notify registered IWriter/s.
*/
class IProcessor {
public:
  /* register IWriter to receive notification once record is processed */
  virtual void registerWriter (const IWriterPtr &writer) = 0;

  /* process the data */
  virtual void process () = 0;
};
typedef std::shared_ptr<IProcessor> IProcessorPtr;

/*
  Stat interface to record statistics
*/
class IStat {
public:
  /* start recording the statistics */
  virtual void start (const std::string &key) = 0;

  /* stop recording the statistics */
  virtual void end (const std::string &key) = 0;

  /* return statistics as string */
  virtual std::string toString () = 0;
};
typedef std::shared_ptr<IStat> IStatPtr;

#endif // __INTERFACE_HPP__
