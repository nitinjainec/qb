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
    using util::copy;
    VLOG ("Creating Trade from ByteBuffer");
    VLOG ("buffer size: " + std::to_string (buffer.size ()));
    VLOG ("required size: " + std::to_string (size ()));
    assert (buffer.size () >= size ());

    const char *ch = buffer.c_str ();
    RecordType rt = static_cast <RecordType> (*ch);
    assert (rt == TRADE);
    ch += sizeof (RecordType);

    copy (symbol, &ch, sizeof (symbol));
    time = ch;
    ch += time.size ();
    copy ((char *)&price, &ch, sizeof (price));
    copy ((char *)&condition, &ch, sizeof (condition));
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
  
  Datetime time;
  char symbol[5];
  double price;
  char condition;
};

#endif
