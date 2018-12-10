#ifndef __SERIALIZER_HPP__
#define __SERIALIZER_HPP__

#include <util/byte_buffer.hpp>

class Serializer {
  ByteBuffer _buffer;
public:

  Serializer (const size_t size = 1024 * 1024)
    : _buffer (size)
  {}

  Serializer (const ByteBuffer &buffer)
    : _buffer (buffer)
  {}

  const ByteBuffer toByteBuffer () const {
    return _buffer;
  }
  
  Serializer& serialize (const char* ch, const size_t size);

  friend Serializer &operator << (Serializer &, const uint32_t);
  friend Serializer &operator << (Serializer &, const size_t);  
  friend Serializer &operator << (Serializer &, const double);
  friend Serializer &operator << (Serializer &, const char);
  friend Serializer &operator << (Serializer &, const ByteBuffer &);
  friend Serializer &operator << (Serializer &, const std::string &);
  template <typename T>
  friend Serializer &operator << (Serializer &, const T&);

  template <typename T>
  friend Serializer &operator << (Serializer &, T&);
  
  Serializer& deSerialize (char* ch, const size_t size);

  friend Serializer &operator >> (Serializer &, uint32_t &);
  friend Serializer &operator >> (Serializer &, size_t &);
  friend Serializer &operator >> (Serializer &, double &);
  friend Serializer &operator >> (Serializer &, char &);  
  friend Serializer &operator >> (Serializer &, ByteBuffer &);
  friend Serializer &operator >> (Serializer &, std::string &);
  template <typename T>
  friend Serializer &operator >> (Serializer &, T&);
};

Serializer& Serializer::serialize (const char *ch, const size_t size) {
  _buffer.append (ch, size);
  return *this;
}

Serializer& operator << (Serializer &out, const uint32_t n) {
  out._buffer.append ((char *)&n, sizeof (n));
  return out;
}

Serializer& operator << (Serializer &out, const size_t n) {
  out._buffer.append ((char *)&n, sizeof (n));
  return out;
}

Serializer& operator << (Serializer &out, const double n) {
  out._buffer.append ((char *)&n, sizeof (n));
  return out;
}

Serializer& operator << (Serializer &out, const char ch) {
  out._buffer.append (ch);
  return out;
}

Serializer& operator << (Serializer &out, const ByteBuffer &buffer) {
  out._buffer.append (buffer);
  return out;
}

Serializer& operator << (Serializer &out, const std::string &str) {
  out._buffer.append (ByteBuffer (str));
  return out;
}

template <typename T>
Serializer& operator << (Serializer &out, const T &t) {
  t.serialize (out);
  return out;
}

template <typename T>
Serializer& operator << (Serializer &out, T &t) {
  t.serialize (out);
  return out;
}

/*-------------------------------------------------------*/

Serializer& Serializer::deSerialize (char *ch, const size_t size) {
  memcpy (ch, _buffer.c_str (), size);
  _buffer.erase (size);
  return *this;
}

Serializer& operator >> (Serializer &out, uint32_t &n) {
  memcpy (&n, out._buffer.c_str (), sizeof (n));
  out._buffer.erase (sizeof (n));
  return out;
}

Serializer& operator >> (Serializer &out, size_t &n) {
  memcpy (&n, out._buffer.c_str (), sizeof (n));
  out._buffer.erase (sizeof (n));
  return out;
}

Serializer& operator >> (Serializer &out, double &n) {
  memcpy (&n, out._buffer.c_str (), sizeof (n));
  out._buffer.erase (sizeof (n));
  return out;
}

Serializer& operator >> (Serializer &out, char &ch) {
  ch = *out._buffer.c_str ();
  out._buffer.erase (sizeof (ch));
  return out;
}

Serializer& operator >> (Serializer &out, ByteBuffer &buffer) {
  buffer = out._buffer;
  out._buffer.erase (out._buffer.size ());
  return out;
}

Serializer& operator >> (Serializer &out, std::string &str) {
  size_t size = out._buffer.find ('\0');
  std::string s (out._buffer.c_str (), size);
  str = s;
  out._buffer.erase (size);
  return out;
}

template <typename T>
Serializer& operator >> (Serializer &out, T &t) {
  t.deSerialize (out);
  return out;
}

#endif
