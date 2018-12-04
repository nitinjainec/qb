#ifndef __STATISTICS_HPP__
#define __STATISTICS_HPP__

#include <vector>

#include "interface.hpp"

class MemoryStat : public IStat {
  static IStatPtr instance;
  MemoryStat () {}
public:
  static IStatPtr getInstance () {
    if (!instance)
      instance.reset (new MemoryStat);
    return instance;
  }
  
  void start (const std::string &key) {

  }

  void end (const std::string &key) {

  }

  std::string toString () {

  }
};
IStatPtr MemoryStat::instance;

class TimeStat : public IStat {
  static IStatPtr instance;
  TimeStat () {}
public:
  static IStatPtr getInstance () {
    if (!instance)
      instance.reset (new TimeStat);
    return instance;
  }
  
  void start (const std::string &key) {
  }

  void end (const std::string &key) {
  }

  std::string toString () {
    
  }
};
IStatPtr TimeStat::instance;

class Stats {
  std::vector <IStatPtr> stats;  
public:
  Stats () {
    stats.push_back (MemoryStat::getInstance ());
    stats.push_back (TimeStat::getInstance ());
  }

  std::vector<IStatPtr>::const_iterator begin () const {
    return stats.begin ();
  }

  std::vector<IStatPtr>::const_iterator end () const {
    return stats.end ();
  }
};

class StatRecorder {
  const std::string &_key;
  static Stats stats;

public:

  StatRecorder (const std::string &key) : _key (key) {
    for (const IStatPtr &stat : stats) {
      stat->start (_key);
    }
  }

  ~StatRecorder () {
    for (const IStatPtr &stat : stats) {
      stat->end (_key);
    }
  }
};

Stats StatRecorder::stats;
#endif
