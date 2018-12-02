#ifndef __UTIL_HPP__
#define __UTIL_HPP__

namespace util {
  /* in place left trime */
  inline void ltrim (std::string &s) {
    s.erase (s.begin (),
	     std::find_if (s.begin (), s.end (),
			   [] (int ch) {return !std::isspace (ch);}));
  }

  /* in place right trim */
  inline void rtrim (std::string &s) {
    s.erase (std::find_if (s.rbegin (), s.rend (),
			   [](int ch) { return !std::isspace (ch); }).base (),
	     s.end ());
  }
  
  /* in place trim from both ends */
  void trim (std::string &s) {
    ltrim (s);
    rtrim (s);
  }
}
#endif
