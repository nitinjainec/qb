#ifndef __BYTE_BUFFER_HPP__
#define __BYTE_BUFFEr_HPP__

#include <memory>
#include <string.h>

/*
  Class to handle binary data and copies data on write
*/
#define BUFFER_CAPACITY 1024
class ByteBuffer {
  size_t _offset;
  size_t _size;
  size_t _capacity;
  typedef std::shared_ptr <char> CPtr;
  CPtr _cptr;

public:
  ByteBuffer (size_t capacity = BUFFER_CAPACITY)
    : _offset (0)
    , _size (0)
    , _capacity (capacity)
    , _cptr (new char[_capacity])
  {}

  ByteBuffer (const char *ch, size_t size)
    : _offset (0)
    , _size (size)
    , _capacity (_size * 2)
    , _cptr (new char[_capacity])
  {
    memcpy (_cptr.get (), ch, _size);
  }

  ByteBuffer (const char *ch)
    : _offset (0)
    , _size (strlen (ch))
    , _capacity (_size * 2)
    , _cptr (new char[_capacity])
  {
    memcpy (_cptr.get (), ch, _size);
    *(_cptr.get () + _size) = '\0';
  }

  ByteBuffer (const std::string &str)
    : _offset (0)
    , _size (str.size ())
    , _capacity (_size * 2)
    , _cptr (new char[_capacity])
  {
    memcpy (_cptr.get (), str.c_str (), _size);
    *(_cptr.get () + _size) = '\0';
  }

  ByteBuffer (const ByteBuffer &buffer, const size_t offset) {
    assert (offset <= buffer._size);
    _offset = buffer._offset;
    _size = buffer._size;
    _capacity = buffer._capacity;
    _cptr = buffer._cptr;
    erase (offset);
  }

  char operator [] (int idx) {
    assert (idx < _size);    
    return *(_cptr.get () + _offset + idx);
  }
  
  ByteBuffer& append (const char *ch, size_t size) {
    if (_capacity - _size - _offset < size) {
      _capacity = std::max ((_size + size) * 2, _capacity);
      CPtr cptr (new char[_capacity]);
      memcpy (cptr.get (), _cptr.get () + _offset, _size);
      _offset = 0;
      _cptr = cptr;
    }
    memcpy (_cptr.get () + _size + _offset, ch, size);
    _size += size;
    return *this;
  }

  ByteBuffer& append (const ByteBuffer &buffer) {
    return append (buffer._cptr.get () + buffer._offset, buffer._size);
  }

  void reserve (size_t capacity) {
    if (_capacity < capacity) {
      _capacity = capacity;
      CPtr cptr (new char[_capacity]);
      memcpy (cptr.get (), _cptr.get () + _offset, _size);
      _offset = 0;
      _cptr = cptr;
    }
  }

  /* erase bytes from begining */
  void erase (size_t size) {
    assert (size <= _size);
    _offset += size;
    _size -= size;
  }
  
  size_t size () const {
    return _size;
  }

  const char *c_str () const {
    return _cptr.get () + _offset;
  }

  size_t capacity () const {
    return _capacity;
  }

  ~ByteBuffer () {
  }
};

#endif
