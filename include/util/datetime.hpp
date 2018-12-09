#ifndef __DATETIME_HPP__
#define __DATETIME_HPP__

struct Datetime {
  char _datetime[24];
  Datetime () {
    memset (_datetime, 'a', size () - 1);
    _datetime [size () - 1] = '\0';
  }
  Datetime (const std::string &datetime)
  {
    assert (datetime.size () == size () - 1);
    memcpy (_datetime, datetime.c_str (), datetime.size ());
    _datetime [size () - 1] = '\0';
  }

  Datetime (const char *ch) {
    memcpy (_datetime, ch, size () - 1);
    _datetime [size () - 1] = '\0';
  }

  Datetime& operator= (const char *ch) {
    memcpy (_datetime, ch, size () - 1);
    _datetime [size () - 1] = '\0';
    return *this;
  }

  std::string toString () {
    return std::string (_datetime);
  }

  ByteBuffer toByteBuffer () {
    return ByteBuffer (_datetime, size ());
  }
  
  static size_t size () {
    return sizeof (_datetime);
  }

  /* serialize the record */
  Serializer& serialize (Serializer& sr) const {
    sr.serialize (_datetime, sizeof (_datetime));
    return sr;
  }

  /* deserialize the record */
  Serializer& deSerialize (Serializer& sr) {
    sr.deSerialize (_datetime, sizeof (_datetime));
    return sr;
  }
};

#endif
