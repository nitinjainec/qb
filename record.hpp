#ifndef __RECORD_HPP__
#define __RECORD_HPP__

#include <stdio.h>
#include <string.h>
#include <sstream>

namespace {

  void copy (char *dest, const char** src, size_t size) {
    memcpy (dest, *src, size);
    *src += size;
  }
}

struct Quote : public Record {
  Quote () {}

  Quote (const ByteBuffer &buffer) {
    VLOG ("Creating Quote from ByteBuffer");
    assert (buffer.size () >= size ());

    const char *ch = buffer.c_str ();
    RecordType rt = static_cast <RecordType> (*ch);
    assert (rt == QUOTE);
    ch += sizeof (RecordType);

    copy (symbol, &ch, sizeof (symbol));
    time = ch;
    ch += time.size ();
    copy ((char *)&bid, &ch, sizeof (bid));
    copy ((char *)&ask, &ch, sizeof (ask));
    copy ((char *)&bsize, &ch, sizeof (bsize));
    copy ((char *)&asize, &ch, sizeof (asize));
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

  static size_t size () {
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
  
  size_t vsize () { return size (); }

  Datetime time;
  char symbol[5];
  double bid;
  double ask;
  uint32_t bsize;
  uint32_t asize;
};

struct Trade : public Record {
  Trade () {}

  Trade (const ByteBuffer &buffer) {
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
    , condition (0xff)
  { strcpy (symbol, symbol_str.c_str ()); }

  static size_t size () {
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
    if (condition != 0xff)
      ss << "," << condition;
    return ss.str ();
  }

  static constexpr RecordType recordType () {
    return TRADE;
  }

  std::string recordTypeName () {
    return "TRADE";
  }
  
  size_t vsize () { return size (); }
  
  Datetime time;
  char symbol[5];
  double price;
  char condition;
};

struct Signal : public Record {
  Signal () {}
  Signal (const ByteBuffer &buffer) {
    VLOG ("Creating Signal from ByteBuffer");
    assert (buffer.size () >= size ());

    const char *ch = buffer.c_str ();
    RecordType rt = static_cast <RecordType> (*ch);
    assert (rt == SIGNAL);
    ch += sizeof (RecordType);

    copy (symbol, &ch, sizeof (symbol));
    VLOG ("symbol: " + std::string (symbol));
    time = ch;
    VLOG ("time: " + time.toString());
    ch += time.size ();
    copy ((char *)&value, &ch, sizeof (value));
    copy ((char *)&code, &ch, sizeof (code));
  }

  Signal (const std::string &time,
	  const std::string &symbol_str,
	  const double &value,
	  const uint32_t code)
    : time (time)
    , value (value)
    , code (code)
  { strcpy (symbol, symbol_str.c_str ()); }
  
  static size_t size () {
    return sizeof (RecordType) + Datetime::size () + sizeof (char[5])
      + sizeof (double) + sizeof (uint32_t);
  }
  
  ByteBuffer toByteBuffer () {
    ByteBuffer buffer (size () * 2);
    RecordType rt = recordType ();
    buffer.append ((char *)&rt, sizeof (RecordType));
    buffer.append (symbol, sizeof (symbol));
    buffer.append (time.toByteBuffer ());
    buffer.append ((char *)&value, sizeof (value));
    buffer.append ((char *)&code, sizeof (code));
    return buffer;
  }

  std::string toString () {
    std::stringstream ss;
    ss << time.toString () << "," << symbol << "," << value << "," << code;
    return ss.str ();
  }

  static constexpr RecordType recordType () {
    return SIGNAL;
  }

  std::string recordTypeName () {
    return "SIGNAL";
  }

  size_t vsize () { return size (); }
 
  Datetime time;
  char symbol[5];
  double value;
  uint32_t code;
};

#endif
