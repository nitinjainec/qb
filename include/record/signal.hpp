#ifndef __SIGNAL_HPP__
#define __SIGNAL_HPP__

#include <stdio.h>
#include <string.h>
#include <sstream>

#include <util/datetime.hpp>
#include <util/util.hpp>

struct Signal : public Record {
  Signal () {}
  Signal (const ByteBuffer &buffer) {
    using util::copy;
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
  
  static size_t ssize () {
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

  size_t size () { return ssize (); }

  std::string symbolName () {
    return symbol;
  }
  
  Serializer& serialize (Serializer &sr) const {
    sr << time;
    sr.serialize (symbol, sizeof (symbol));
    sr << value;
    sr << code;
    return sr;
  }

  Serializer& deSerialize (Serializer &sr) {
    sr >> time;
    sr.deSerialize (symbol, sizeof (symbol));
    sr >> value;
    sr >> code;
    return sr;
  }
  
  Datetime time;
  char symbol[5];
  double value;
  uint32_t code;
};
#endif
