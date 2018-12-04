#ifndef __STAT_RECORDER_HPP__
#define __STAT_RECORDER_HPP__

#include <assert.h>
#include <chrono>
#include <sstream>
#include <vector>

#include <interface.hpp>
#include <statistics/memory_stat.hpp>
#include <statistics/time_stat.hpp>
#include <util/logger.hpp>
#include <util/util.hpp>

class Stats {
public:
  std::vector <IStatPtr> stats;  

  Stats () {
    stats.push_back (TimeStat::getInstance ());    
    stats.push_back (MemoryStat::getInstance ());
  }

  std::vector<IStatPtr>::const_iterator begin () const {
    return stats.begin ();
  }

  std::vector<IStatPtr>::const_iterator end () const {
    return stats.end ();
  }
};

class StatRecorder {
  const std::string _key;
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

  static void display (std::ostream& stream) {
    VSLOG ("display, stats size: " + std::to_string (stats.stats.size ()));
    for (const IStatPtr &stat : stats) {
      stream << stat->toString ();
    }
  }
};

Stats StatRecorder::stats;

#endif
