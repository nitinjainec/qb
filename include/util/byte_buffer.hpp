#ifndef __BYTE_BUFFER_HPP__
#define __BYTE_BUFFER_HPP__

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

  void _increaseByCopy (const size_t size) {
    _capacity = std::max ((_size + size) * 2, _capacity);
    CPtr cptr (new char[_capacity]);
    memcpy (cptr.get (), _cptr.get () + _offset, _size);
    _offset = 0;
    _cptr = cptr;
  }

  void _increase (const size_t size) {
    if (_capacity - _size - _offset < size) {
      if (_cptr.use_count () > 1
	  || _capacity - _size < size) {
	_increaseByCopy (size);
      }
      else {
	memcpy (_cptr.get (), _cptr.get () + _offset, _size);
	_offset = 0;
      }
    }
  }

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

  ByteBuffer (const ByteBuffer &buffer, const size_t size,
	      const size_t offset = 0) {
    assert (offset <= buffer._size);
    assert (size <= buffer._size);
    assert (offset <= size);
    
    _offset = buffer._offset + offset;
    _size = size;
    _capacity = buffer._capacity;
    _cptr = buffer._cptr;
  }

  char operator [] (int idx) {
    assert (idx < _size);    
    return *(_cptr.get () + _offset + idx);
  }

  ByteBuffer& append (const char *ch, size_t size) {
    _increase (size);
    memcpy (_cptr.get () + _size + _offset, ch, size);
    _size += size;
    return *this;
  }

  ByteBuffer& append (const ByteBuffer &buffer) {
    return append (buffer._cptr.get () + buffer._offset, buffer._size);
  }

  /* 
   * Returns char * to copy given size of data
   * Saves extra copy of data while reading from source.
   * WARNING: Use it bit caution. Writing more that specified size will result
   * in undefine behaviour.
   */
  char * toAppend (const size_t size) {
    _increase (size);
    return (_cptr.get () + _offset + _size);
  }

  /*
   * Invoke after toAppend to update internal structure of byte_buffer
   * If function is not invoked appended size will be treated as 0
   */
  void appended (const size_t size) {
    assert (_offset + _size + size <= _capacity);
    _size += size;
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
