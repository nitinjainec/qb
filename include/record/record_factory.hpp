#ifndef __RECORD_FACTORY_HPP__
#define __RECORD_FACTORY_HPP__

#include <interface.hpp>
#include <util/logger.hpp>
#include <record/quote.hpp>
#include <record/trade.hpp>
#include <record/signal.hpp>

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
    VSLOG (fields[3]);
    return RecordPtr (new Signal (fields[0],
				  fields[1],
				  std::stod (fields[2]),
				  std::stoi (fields[3])));
  }

  static size_t recordSize (const RecordType rt) {
    switch (rt) {
    case Quote::recordType (): return Quote::ssize ();
    case Trade::recordType (): return Trade::ssize ();
    case Signal::recordType (): return Signal::ssize ();
    }
    VSLOG (std::to_string (rt));
    assert (false);
  }

  static RecordPtr createQuote (const ByteBuffer &buffer) {
    assert (*buffer.c_str () == QUOTE);
    return RecordPtr (new Quote (buffer));
  }

  static RecordPtr createTrade (const ByteBuffer &buffer) {
    assert (*buffer.c_str () == TRADE);
    return RecordPtr (new Trade (buffer));
  }

  static RecordPtr createSignal (const ByteBuffer &buffer) {
    assert (*buffer.c_str () == SIGNAL);
    return RecordPtr (new Signal (buffer));
  }
public:
  static RecordPtr create (const std::vector<ByteBuffer> &buffers) {
    std::vector <std::string> fields;
    for (auto &buffer : buffers) {
      fields.push_back (std::string (buffer.c_str (), buffer.size ()));
    }
    create (fields);
  }
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

  static bool canCreate (const ByteBuffer &buffer) {
    VSLOG ("canCreate with buffer size: " + std::to_string (buffer.size ()));
    if (buffer.size () == 0)
      return false;

    RecordType rt = static_cast <RecordType>(*buffer.c_str ());
    VSLOG ("Expected recordSize: " + std::to_string (recordSize (rt)));
    return buffer.size () >= recordSize (rt);
  }
  
  static RecordPtr create (const ByteBuffer &buffer) {
    assert (canCreate (buffer));
    const char *ch = buffer.c_str ();

    VSLOG ("Creating record from ByteBuffer: " + std::to_string (buffer.size ()));
    RecordType rt = static_cast <RecordType>(*buffer.c_str ());
    switch (rt) {
    case Quote::recordType (): return createQuote (buffer);
    case Trade::recordType (): return createTrade (buffer);
    case Signal::recordType (): return createSignal (buffer);
    }
  }
};

#endif
