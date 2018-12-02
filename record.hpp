#ifndef __RECORD_HPP__
#define __RECORD_HPP__

#include <stdio.h>
#include <string.h>

struct Quote : public Record {
  Quote () {}

  Quote (const Buffer &buffer) {
    const char *ch = buffer.c_str ();

    time = ch;
    ch += time.size ();

    memcpy (symbol, ch, sizeof (symbol));
    ch += sizeof (symbol);

    const double *dptr = reinterpret_cast <const double *> (ch);
    bid = *dptr;
    ch += sizeof (double);

    dptr = reinterpret_cast <const double *> (ch);
    ask = *dptr;
    ch += sizeof (double);
    
    const uint32_t *uiptr = reinterpret_cast <const uint32_t *> (ch);
    bsize = *uiptr;
    ch += sizeof (uint32_t);

    uiptr = reinterpret_cast <const uint32_t *> (ch);
    asize = *uiptr;
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
  { strcpy (symbol, symbol_str.c_str ()); }

  static size_t size () {
    return sizeof (RecordType) + Datetime::size () + sizeof (char [5])
      + sizeof (double) + sizeof (double) + sizeof (uint32_t)
      + sizeof (uint32_t);
  }
  
  Buffer toBinaryBuffer () {
    char buffer[size ()];
    char *dest = buffer;

    *dest = recordType ();
    dest += sizeof (RecordType);

    memcpy (dest, &time, time.size ());
    dest += time.size ();

    memcpy (dest, symbol, sizeof (symbol));
    dest += sizeof (symbol);

    memcpy (dest, &bid, sizeof (bid));
    dest += sizeof (bid);

    memcpy (dest, &ask, sizeof (ask));
    dest += sizeof (ask);

    memcpy (dest, &bsize, sizeof (bsize));
    dest += sizeof (bsize);

    memcpy (dest, &asize, sizeof (asize));
    dest += sizeof (asize);

    return buffer;
  }

  std::string toString () {
    return time.toString ()
      + "," + symbol
      + "," + std::to_string (bid)
      + "," + std::to_string (ask)
      + "," + std::to_string (bsize)
      + "," + std::to_string (asize);
  }
  
  static constexpr RecordType recordType () {
    return QUOTE;
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

  Trade (const Buffer &buffer) {
    const char *ch = buffer.c_str ();

    time = ch;
    ch += time.size ();

    memcpy (symbol, ch, sizeof (symbol));
    ch += sizeof (symbol);

    const double *dptr = reinterpret_cast <const double *> (ch);
    price = *dptr;
    ch += sizeof (double);

    condition = *ch;
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
  
  Buffer toBinaryBuffer () {
    char buffer[size ()];
    char *dest = buffer;

    *dest = recordType ();
    dest += sizeof (RecordType);
    
    memcpy (dest, &time, time.size ());
    dest += time.size ();

    memcpy (dest, symbol, sizeof (symbol));
    dest += sizeof (symbol);

    memcpy (dest, &price, sizeof (price));
    dest += sizeof (price);

    *dest = condition;
    return buffer;
  }

  std::string toString () {
    return (condition == 0xff)
      ? time.toString () + "," + symbol + "," + std::to_string (price)
      : (time.toString () + "," + symbol + "," + std::to_string (price)
	 + "," + std::string (condition, 1));
  }

  static constexpr RecordType recordType () {
    return TRADE;
  }
  
  size_t vsize () { return size (); }
  
  Datetime time;
  char symbol[5];
  double price;
  char condition;
};

struct Signal : public Record {
  Signal () {}
  Signal (const Buffer &buffer) {
    const char *ch = buffer.c_str ();

    time = ch;
    ch += time.size ();

    memcpy (symbol, ch, sizeof (symbol));
    ch += sizeof (symbol);

    const double *dptr = reinterpret_cast <const double *> (ch);
    value = *dptr;
    ch += sizeof (double);

    const uint32_t *uiptr = reinterpret_cast <const uint32_t *> (ch);
    code = *uiptr;
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
  
  Buffer toBinaryBuffer () {
    char buffer[size ()];
    char *dest = buffer;

    *dest = recordType ();
    dest += sizeof (RecordType);
    
    memcpy (dest, &time, time.size ());
    dest += time.size ();

    memcpy (dest, symbol, sizeof (symbol));
    dest += sizeof (symbol);

    memcpy (dest, &value, sizeof (value));
    dest += sizeof (value);
    
    memcpy (dest, &code, sizeof (code));
    return buffer;
  }

  std::string toString () {
    return time.toString ()
      + "," + symbol
      + "," + std::to_string (value)
      + "," + std::to_string (code);
  }

  static constexpr RecordType recordType () {
    return SIGNAL;
  }

  size_t vsize () { return size (); }
 
  Datetime time;
  char symbol[5];
  double value;
  uint32_t code;
};

#endif
