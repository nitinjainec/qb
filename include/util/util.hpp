#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <algorithm>

#include <unistd.h>
#include <sys/resource.h>

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

  void copy (char *dest, const char** src, size_t size) {
    memcpy (dest, *src, size);
    *src += size;
  }
  
  namespace linux {
    /* Returns the current resident set size or 0 if the value cannot be read. */
    size_t getCurrentRSS ( )
    {
      long rss = 0L;
      size_t c_rss = 0;
      FILE* fp = NULL;
      if ((fp = fopen( "/proc/self/statm", "r" )) != NULL) {
	if (fscanf (fp, "%*s%ld", &rss) == 1)
	  c_rss = rss * (size_t) sysconf (_SC_PAGESIZE);
	fclose( fp );
      }
      return c_rss; 
    }

    /* Returns max resident set size measured in bytes */
    size_t getMaxRSS( )
    {
      struct rusage rusage;
      getrusage( RUSAGE_SELF, &rusage );
      return (size_t)(rusage.ru_maxrss * 1024L);
    }
  }
}
#endif
