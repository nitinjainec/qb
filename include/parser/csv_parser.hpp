#ifndef __CSV_PARSER_HPP__
#define __CSV_PARSER_HPP__

#include <algorithm>
#include <vector>
#include <sstream>

#include <interface.hpp>
#include <record_factory/csv_record_factory.hpp>
#include <statistics/stat_recorder.hpp>
#include <util/logger.hpp>
#include <util/util.hpp>

/*
 * Class to parse CSV data received from CSVReader
 */
class CSVParser : public IParser {
  IReaderPtr _reader;
  CSVRecordFactoryPtr _factory;

  RecordPtr parseBufferToRecord (ByteBuffer &buffer) {
    StatRecorder sr ("Parsing record from csv");
    std::vector <std::string> fields;
    for (int idx = buffer.find (','); idx != -1; idx = buffer.find (',')) {
      fields.push_back (util::trim (std::string (buffer.c_str (), idx)));
      buffer.erase (idx+1);
    }
    fields.push_back (util::trim
		      (std::string(buffer.c_str (), buffer.size ())));
    return _factory->createRecord (fields);
  }

public:
  CSVParser (const IReaderPtr &reader, const CSVRecordFactoryPtr &factory)
    : _reader (reader)
    , _factory (factory)
  {}

  /* Parses the data and returns next record */
  RecordPtr nextRecord () {
    StatRecorder sr ("Parse csv to Record");
    assert (!eor ());
    ByteBuffer buffer = _reader->getData ();
    return parseBufferToRecord (buffer);
  }

  /* Returns true when end of record is reached */
  bool eor () {
    return _reader->eod ();
  }
};

#endif // __CSV_PARSER_HPP__
