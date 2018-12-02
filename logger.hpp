#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <assert.h>
#include <iostream>
#include <map>
#include <memory>


#ifdef DEBUG
#define VLOG(x) Logger::get ()->log (typeid(*this).name(), Logger::VERBOSE, x)
#define VSLOG(x) Logger::get ()->log ("STATIC", Logger::VERBOSE, x)
#else
#define VLOG(x) ;
#define VSLOG(x) ;
#endif

#define DLOG(x) Logger::get ()->log (typeid(*this).name(), Logger::DEBUG, x)

/*
  Simple logger class to log message on output stream
*/
class Logger {
  typedef std::shared_ptr <Logger> LoggerPtr;
  static std::shared_ptr <Logger> instance;

public:
  enum Level {
    VERBOSE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
  };

private:
  static std::map <Level, std::string> level_to_string;
  
  Logger () {
    if (level_to_string.size () == 0) {
      level_to_string[VERBOSE] = "VERBOSE";
      level_to_string[DEBUG] = "DEBUG";
      level_to_string[INFO] = "INFO";
      level_to_string[WARNING] = "WARNING";
      level_to_string[ERROR] = "ERROR";
      level_to_string[FATAL] = "FATAL";
    }
  }
  
public:

  /* Create logger instance if it does not exist */
  static LoggerPtr create () {
    // TODO: Need to take lock if we need to make it thread safe
    if (!instance) {
      instance.reset (new Logger ());
    }
    return instance;
  }

  /* Returns logger instance */
  static LoggerPtr get () {
    assert (instance);
    return instance;
  }
  
  /* Logs the message on the given output stream */
  void log (const std::string &module, Level level, const std::string &msg) {
    std::cout << module << " - " << level_to_string [level] << ": " << msg << "\n";
  }
};

typedef std::shared_ptr <Logger> LoggerPtr;
LoggerPtr Logger::instance;
std::map <Logger::Level, std::string> Logger::level_to_string;

#endif
