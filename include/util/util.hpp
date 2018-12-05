#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <dirent.h>
#include <sys/resource.h>
#include <unistd.h>

#include <algorithm>

namespace util {
  /* in place left trime */
  inline void ltrim (std::string &s) {
    s.erase (s.begin (),
	     std::find_if (s.begin (), s.end (),
			   [] (int ch) {return !std::isspace (ch);}));
    //return s;
  }

  /* in place right trim */
  inline void rtrim (std::string &s) {
    s.erase (std::find_if (s.rbegin (), s.rend (),
			   [](int ch) { return !std::isspace (ch); }).base (),
	     s.end ());
  }
  
  /* in place trim from both ends */
  inline void trim (std::string &s) {
    ltrim (s);
    rtrim (s);
  }

  /* in place left trime */
  inline std::string &ltrim (std::string &&s) {
    s.erase (s.begin (),
	     std::find_if (s.begin (), s.end (),
			   [] (int ch) {return !std::isspace (ch);}));
    return s;
  }

  /* in place right trim */
  inline std::string &rtrim (std::string &&s) {
    s.erase (std::find_if (s.rbegin (), s.rend (),
			   [](int ch) { return !std::isspace (ch); }).base (),
	     s.end ());
    return s;
  }

  /* in place trim from both ends */
  inline std::string& trim (std::string &&s) {
    ltrim (s);
    rtrim (s);
    return s;
  }

  void copy (char *dest, const char** src, size_t size) {
    memcpy (dest, *src, size);
    *src += size;
  }
  
  /* returns random string */
  std::string randomString (size_t size) {
    auto randomChar = []() -> char {
      const char charset[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
      const size_t max_index = (sizeof(charset) - 1);
      return charset[ rand() % max_index ];
    };
    srand (time (NULL));
    std::string str (size, 0);
    std::generate_n (str.begin(), size, randomChar);
    return str;
  }

  namespace linux {
    /* Returns the current resident set size or 0 if the value cannot be read. */
    size_t getCurrentRSS () {
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
    size_t getMaxRSS () {
      struct rusage rusage;
      getrusage( RUSAGE_SELF, &rusage );
      return (size_t)(rusage.ru_maxrss * 1024L);
    }

    /* returns list of files in a directory with given prefix */
    std::vector <std::string> listDirectory (const std::string& directory, const std::string &prefix = "") {
      std::vector <std::string> files;
      DIR* dirp = opendir (directory.c_str());
      struct dirent * dp;
      while ((dp = readdir (dirp)) != NULL) {
	std::string file (dp->d_name);
	auto res = std::mismatch(prefix.begin(), prefix.end(), file.begin());
	if (res.first == prefix.end ())
	  files.push_back (file);
      }
      closedir(dirp);
      return files;
    }
  }
}
#endif
