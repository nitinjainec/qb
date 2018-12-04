#ifndef __BYTE_BUFFER_HPP__
#define __BYTE_BUFFEr_HPP__

#include <string.h>

/*
  Class to handle binary data
*/
#define BUFFER_CAPACITY 1024
class ByteBuffer {
  size_t _size;
  size_t _capacity;
  char *_cptr;

public:
  ByteBuffer (size_t capacity = BUFFER_CAPACITY)
    : _size (0)
    , _capacity (capacity)
    , _cptr (new char[_capacity])
  {}

  ByteBuffer (const char *ch, size_t size)
    : _size (size)
    , _capacity (_size * 2)
    , _cptr (new char[_capacity])
  {
    memcpy (_cptr, ch, _size);
  }

  ByteBuffer (const char *ch)
    : _size (strlen (ch))
    , _capacity (_size * 2)
    , _cptr (new char[_capacity])
  {
    memcpy (_cptr, ch, _size);
    _cptr[_size] = '\0';
  }

  ByteBuffer (const std::string &str)
    : _size (str.size ())
    , _capacity (_size * 2)
    , _cptr (new char[_capacity])
  {
    memcpy (_cptr, str.c_str (), _size);
    _cptr[_size] = '\0';
  }
  
  char operator [] (int idx) {
    assert (idx < _size);    
    return *(_cptr + idx);
  }
  
  ByteBuffer& append (const char *ch, size_t size) {
    if (_capacity - _size < size) {
      _capacity = (_size + size) * 2;
      char *cptr = new char[_capacity];
      memcpy (cptr, _cptr, _size);
      delete [] _cptr;
      _cptr = cptr;
    }
    memcpy (_cptr + _size, ch, size);
    _size += size;
    return *this;
  }

  ByteBuffer& append (const ByteBuffer &buffer) {
    return append (buffer._cptr, buffer._size);
  }

  ByteBuffer (const ByteBuffer &buffer) {
    _cptr = new char [buffer._capacity];
    _capacity = buffer._capacity;
    _size = buffer._size;
    memcpy (_cptr, buffer._cptr, _size);
  }

  void reserve (size_t capacity) {
    if (_capacity < capacity) {
      _capacity = capacity;
      char *cptr = new char[_capacity];
      memcpy (cptr, _cptr, _size);
      delete _cptr;
      _cptr = cptr;
    }
  }

  /* erase bytes from begining */
  void erase (size_t size) {
    char *ch = new char [1024];
    memcpy (ch, _cptr+size, _size - size);
    memcpy (_cptr, ch, _size - size);
    //memcpy (_cptr, _cptr+size, _size - size);
    _size -= size;
  }
  
  size_t size () const {
    return _size;
  }

  const char *c_str () const {
    return _cptr;
  }

  size_t capacity () const {
    return _capacity;
  }

  ~ByteBuffer () {
    delete [] _cptr;
  }
};

#endif
