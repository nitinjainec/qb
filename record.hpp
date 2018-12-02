#ifndef __RECORD_HPP__
#define __RECORD_HPP__

#include <stdio.h>
#include <string.h>

struct Quote : public Record {
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

  size_t size () {
    return time.size () + sizeof (symbol) + sizeof (double)
      + sizeof (double) + sizeof (uint32_t) + sizeof (uint32_t);
  }
  
  Buffer toBinaryBuffer () {
    //time.toBinaryBuffer () + symbol +
    char buffer[size ()];
    char *dest = buffer;
    
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
  
  Datetime time;
  char symbol[5];
  double bid;
  double ask;
  uint32_t bsize;
  uint32_t asize;
};

struct Trade : public Record {
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

  size_t size () {
    return time.size () + sizeof (symbol) + sizeof (double) + sizeof (char);
  }
  
  Buffer toBinaryBuffer () {
    char buffer[size ()];
    char *dest = buffer;
    
    memcpy (dest, &time, time.size ());
    dest += time.size ();

    memcpy (dest, symbol, sizeof (symbol));
    dest += sizeof (symbol);

    memcpy (dest, &price, sizeof (price));
    dest += sizeof (price);

    *dest = condition;
    return buffer;
  }
  
  Datetime time;
  char symbol [5];
  double price;
  char condition;
};

struct Signal : public Record {
  Signal (const std::string &time,
	  const std::string &symbol_str,
	  const double &value,
	  const int code)
    : time (time)
    , value (value)
    , code (code)
  { strcpy (symbol, symbol_str.c_str ()); }

  size_t size () {
    return time.size () + sizeof (symbol) + sizeof (double) + sizeof (uint32_t);
  }
  
  Buffer toBinaryBuffer () {
    char buffer[size ()];
    char *dest = buffer;
    
    memcpy (dest, &time, time.size ());
    dest += time.size ();

    memcpy (dest, symbol, sizeof (symbol));
    dest += sizeof (symbol);

    memcpy (dest, &value, sizeof (value));
    dest += sizeof (value);
    
    memcpy (dest, &code, sizeof (code));
    return buffer;
  }
    
  Datetime time;
  char symbol [5];
  double value;
  int code;
};

#endif
