#ifndef __RECORD_HPP__
#define __RECORD_HPP__

#include <stdio.h>
#include <string.h>
#include <sstream>

#include <util/datetime.hpp>
#include <util/util.hpp>

struct Quote : public Record {
  Quote () {}

  Quote (const ByteBuffer &buffer) {
    Serializer sr (buffer);
    deSerialize (sr);
  }

  Quote (const std::string &time,
	 const std::string &symbol_str,
	 const double &bid,
	 const double &ask,
	 const int bsize,
	 const int asize)
    : time (time)
    , bid (bid)
    , ask (ask)
    , bsize (bsize)
    , asize (asize)
  {
    strcpy (symbol, symbol_str.c_str ());
    VLOG ("Quote constructed");
    VLOG ("Symbol " + std::string (symbol, sizeof (symbol)));
  }

  static size_t ssize () {
    return sizeof (RecordType) + Datetime::size () + sizeof (char [5])
      + sizeof (double) + sizeof (double) + sizeof (uint32_t)
      + sizeof (uint32_t);
  }
  
  ByteBuffer toByteBuffer () {
    ByteBuffer buffer (size () * 2);
    RecordType rt = recordType ();
    buffer.append ((char *)&rt, sizeof (RecordType));
    buffer.append (symbol, sizeof (symbol));
    buffer.append (time.toByteBuffer ());
    buffer.append ((char *)&bid, sizeof (bid));
    buffer.append ((char *)&ask, sizeof (ask));
    buffer.append ((char *)&bsize, sizeof (bsize));
    buffer.append ((char *)&asize, sizeof (asize));
    return buffer;
  }

  std::string toString () {
    return time.toString ()
      + "," + symbol
      + "," + std::to_string (bid)
      + "," + std::to_string (ask)
      + "," + std::to_string (bsize)
      + "," + std::to_string (asize);
    //+ std::string ('\0', 1);
  }
  
  static constexpr RecordType recordType () {
    return QUOTE;
  }

  std::string recordTypeName () {
    return "QUOTE";
  }

  std::string symbolName () {
    return symbol;
  }

  size_t size () { return ssize (); }

  Serializer &serialize (Serializer &sr) const {
    sr << static_cast<uint32_t>(recordType ());
    sr << time;
    sr.serialize (symbol, sizeof (symbol));
    sr << bid;
    sr << ask;
    sr << bsize;
    sr << asize;
    return sr;
  }

  Serializer &deSerialize (Serializer &sr) {
    uint32_t rt;
    sr >> rt;
    assert (rt == recordType ());
    sr >> time;
    sr.deSerialize (symbol, sizeof (symbol));
    sr >> bid;
    sr >> ask;
    sr >> bsize;
    sr >> asize;
    return sr;
  }

  Datetime time;
  char symbol[5];
  double bid;
  double ask;
  uint32_t bsize;
  uint32_t asize;
};




#endif
