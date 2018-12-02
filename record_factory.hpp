#ifndef __RECORD_FACTORY_HPP__
#define __RECORD_FACTORY_HPP__

#include "interface.hpp"
#include "logger.hpp"
#include "record.hpp"

/*
  Factory class to create Record based on fields
*/
class RecordFactory {

  static RecordPtr createQuote (const std::vector<std::string> &fields) {
    assert (fields.size () == 6);
    VSLOG ("Creating Quote record");
    return RecordPtr (new Quote (fields[0],
				 fields[1],
				 std::stod (fields[2]),
				 std::stod (fields[3]),
				 std::stoi (fields[4]),
				 std::stoi (fields[5])));
  }

  static RecordPtr createTrade (const std::vector<std::string> &fields) {
    assert (fields.size () == 3 || fields.size () == 4);
    VSLOG ("Creating Trade record");
    return (fields.size () == 3 || fields[3].size () == 0)
      ? RecordPtr (new Trade (fields[0],
			      fields[1],
			      std::stod (fields[2])))
      : RecordPtr (new Trade (fields[0],
			      fields[1],
			      std::stod (fields[2]),
			      fields[3][0]));
  }

  static RecordPtr createSignal (const std::vector<std::string> &fields) {
    assert (fields.size () == 4);
    VSLOG ("Creating Signal record");
    return RecordPtr (new Signal (fields[0],
				  fields[1],
				  std::stod (fields[2]),
				  std::stoi (fields[3])));
  }

  static size_t recordSize (const RecordType rt) {
    switch (rt) {
    case Quote::recordType (): return Quote::size ();
    case Trade::recordType (): return Trade::size ();
    case Signal::recordType (): return Signal::size ();
    }
  }

  static RecordPtr createQuote (const Buffer &buffer) {
    assert (*buffer.c_str () == QUOTE);
    return RecordPtr (new Quote (buffer.c_str () + 1));
  }

  static RecordPtr createTrade (const Buffer &buffer) {
    assert (*buffer.c_str () == TRADE);
    return RecordPtr (new Trade (buffer.c_str () + 1));
  }

  static RecordPtr createSignal (const Buffer &buffer) {
    assert (*buffer.c_str () == SIGNAL);
    return RecordPtr (new Signal (buffer.c_str () + 1));
  }
public:
  static RecordPtr create (const std::vector<std::string> &fields) {
    assert (fields.size () == 3
	    || fields.size () == 4
	    || fields.size () == 6);
    
    if (fields.size () == 6)
      return createQuote (fields);

    if (fields.size () == 3
	|| fields[3].size () == 1
	|| fields[3].size () == 0)
      return createTrade (fields);
    
    return createSignal (fields);
  }

  static bool canCreate (const Buffer &buffer, const size_t size) {
    VSLOG ("canCreate with buffer size: " + std::to_string (size));
    if (size == 0)
      return false;

    RecordType rt = static_cast <RecordType>(*buffer.c_str ());
    VSLOG ("Expected recordSize: " + std::to_string (recordSize (rt)));
    return size >= recordSize (rt);
  }
  
  static RecordPtr create (const Buffer &buffer, size_t &size) {
    assert (canCreate (buffer, size));
    const char *ch = buffer.c_str ();

    RecordType rt = static_cast <RecordType>(*buffer.c_str ());
    switch (rt) {
    case Quote::recordType (): return createQuote (buffer);
    case Trade::recordType (): return createTrade (buffer);
    case Signal::recordType (): return createSignal (buffer);
    }
  }
};

#endif
