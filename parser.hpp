#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "interface.hpp"
#include "logger.hpp"
#include "record_factory.hpp"
#include "util.hpp"

class CSVParser : public IParser {
  IReaderPtr reader;
  std::string delimeter;

  RecordPtr parseBufferToRecord (const Buffer &buffer) {
    std::vector <std::string> fields;
    std::istringstream s (buffer);
    for (std::string field; std::getline (s, field, ','); ) {
      util::trim (field);
      fields.push_back (field);
    }
    RecordPtr record;
    try {
      record = RecordFactory::create (fields);
    }
    catch (const std::exception& ex) {
      std::cout << "Caught in parser: " << ex.what () << std::endl;
      std::cout << buffer << std::endl;
      exit (0);
    }
    return record;
  }

public:
  CSVParser (const IReaderPtr &reader,
	     const std::string &delimeter = ",")
    : reader (reader)
    , delimeter (delimeter)
  {
    DLOG ("CSVParser constructed");
  }
  
  RecordPtr nextRecord () {
    assert (!eor ());
    const Buffer & buffer = reader->getData ();
    return parseBufferToRecord (buffer);
  }

  bool eor () {
    return reader->eod ();
  }
};

#endif
