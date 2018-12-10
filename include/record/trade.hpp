#ifndef __TRADE_HPP__
#define __TRADE_HPP__

#include <stdio.h>
#include <string.h>
#include <sstream>

#include <util/datetime.hpp>
#include <util/util.hpp>

struct Trade : public Record {
  static char const NO_COND = ' ';
  Trade () {}

  Trade (const ByteBuffer &buffer) {
    Serializer sr (buffer);
    deSerialize (sr);
  }

  Trade (const std::string &time,
	 const std::string &symbol_str,
	 const double &price,
	 const char condition)
    : time (time)
    , price (price)
    , condition (condition)
  { strcpy (symbol, symbol_str.c_str ()); }

  Trade (const std::string &time,
	 const std::string &symbol_str,
	 const double &price)
    : time (time)
    , price (price)
    , condition (NO_COND)
  { strcpy (symbol, symbol_str.c_str ()); }

  static size_t ssize () {
    return sizeof (RecordType) + Datetime::size () + sizeof (char [5])
      + sizeof (double) + sizeof (char);
  }
  
  ByteBuffer toByteBuffer () {
    ByteBuffer buffer (size () * 2);
    RecordType rt = recordType ();
    buffer.append ((char *)&rt, sizeof (RecordType));
    buffer.append (symbol, sizeof (symbol));
    buffer.append (time.toByteBuffer ());
    buffer.append ((char *)&price, sizeof (price));
    buffer.append ((char *)&condition, sizeof (condition));
    return buffer;
  }

  std::string toString () {
    std::stringstream ss;
    ss << time.toString () << "," << symbol << "," << price;
    if (condition != NO_COND)
      ss << "," << condition;
    return ss.str ();
  }

  static constexpr RecordType recordType () {
    return TRADE;
  }

  std::string recordTypeName () {
    return "TRADE";
  }
  
  size_t size () { return ssize (); }

  std::string symbolName () {
    return symbol;
  }

  Serializer& serialize (Serializer &sr) const {
    sr << static_cast<uint32_t>(recordType ());
    sr << time;
    sr.serialize (symbol, sizeof (symbol));
    sr << price;
    sr << condition;
    return sr;
  }

  Serializer& deSerialize (Serializer &sr) {
    uint32_t rt;
    sr >> rt;
    assert (rt == recordType ());
    sr >> time;
    sr.deSerialize (symbol, sizeof (symbol));
    sr >> price;
    sr >> condition;
    return sr;    
  }
  
  Datetime time;
  char symbol[5];
  double price;
  char condition;
};

#endif
