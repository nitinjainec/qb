#ifndef __BINARY_RECORD_FACTORY_HPP__
#define __BINARY_RECORD_FACTORY_HPP__

#include <interface.hpp>
#include <util/logger.hpp>
#include <record/quote.hpp>
#include <record/trade.hpp>
#include <record/signal.hpp>

/*
  Binary Record factory class to create Record based on ByteBuffer
*/
class BinaryRecordFactory : public IRecordFactory <ByteBuffer> {
  static size_t recordSize (const RecordType rt) {
    switch (rt) {
    case Quote::recordType (): return Quote::ssize ();
    case Trade::recordType (): return Trade::ssize ();
    case Signal::recordType (): return Signal::ssize ();
    }
    assert (false);
  }

  RecordPtr createQuote (const ByteBuffer &buffer) {
    assert (*buffer.c_str () == QUOTE);
    return RecordPtr (new Quote (buffer));
  }

  RecordPtr createTrade (const ByteBuffer &buffer) {
    assert (*buffer.c_str () == TRADE);
    return RecordPtr (new Trade (buffer));
  }

  RecordPtr createSignal (const ByteBuffer &buffer) {
    assert (*buffer.c_str () == SIGNAL);
    return RecordPtr (new Signal (buffer));
  }
public:

  /* Returns true if Record can be created with given ByteBuffer */
  bool canCreateRecord (const ByteBuffer &buffer) {
    if (buffer.size () == 0)
      return false;

    RecordType rt = static_cast <RecordType>(*buffer.c_str ());
    return buffer.size () >= recordSize (rt);
  }

  /* Creates the Record with given ByteBuffer */
  RecordPtr createRecord (const ByteBuffer &buffer) {
    assert (canCreateRecord (buffer));
    const char *ch = buffer.c_str ();

    RecordType rt = static_cast <RecordType>(*buffer.c_str ());
    switch (rt) {
    case Quote::recordType (): return createQuote (buffer);
    case Trade::recordType (): return createTrade (buffer);
    case Signal::recordType (): return createSignal (buffer);
    }
  }
};
typedef IRecordFactory<ByteBuffer>::Ptr BinaryRecordFactoryPtr;

#endif // __BINARY_RECORD_FACTORY_HPP__
