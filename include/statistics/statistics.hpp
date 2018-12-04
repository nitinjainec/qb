#ifndef __STATISTICS_HPP__
#define __STATISTICS_HPP__

#include <assert.h>
#include <chrono>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <sys/resource.h>

#include "interface.hpp"
#include "logger.hpp"

namespace Linux {
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

class MemoryStat : public IStat {
  struct Stat {
    Stat () : count (0), maxCurrentSizeInc (0), maxPeakSizeInc (0),
	      maxCurrentSize (0), maxPeakSize (0), started (false),
	      startCurrentSize (0), startPeakSize (0) {}

    /* stat */
    uint32_t count;    
    size_t maxCurrentSizeInc;
    size_t maxPeakSizeInc;
    size_t maxCurrentSize;
    size_t maxPeakSize;

    /* internal */
    bool started;
    size_t startCurrentSize;
    size_t startPeakSize;    
  };

  static IStatPtr instance;

  std::map <std::string, Stat> _key_to_stat;
  
  void updateStat (Stat &stat) {
    size_t currentSize = Linux::getCurrentRSS ();
    size_t peakSize    = Linux::getMaxRSS ();

    size_t currentSizeInc = currentSize - stat.startCurrentSize;
    size_t peakSizeInc = peakSize - stat.startPeakSize;

    if (stat.maxCurrentSizeInc < currentSizeInc)
      stat.maxCurrentSizeInc = currentSizeInc;

    if (stat.maxPeakSizeInc < peakSizeInc)
      stat.maxPeakSizeInc = peakSizeInc;

    if (stat.maxCurrentSize < currentSize)
      stat.maxCurrentSize = currentSize;

    if (stat.maxPeakSize < peakSize)
      stat.maxPeakSize = peakSize;	
  }

public:
  static IStatPtr getInstance () {
    if (!instance)
      instance.reset (new MemoryStat);
    return instance;
  }
  
  void start (const std::string &key) {
    VLOG ("Start Key: " + key);
    if (_key_to_stat.find (key) == _key_to_stat.end ())
      _key_to_stat [key] = Stat ();

    Stat &stat = _key_to_stat [key];
    assert (!stat.started);
    stat.started = true;
    ++stat.count;
    stat.startCurrentSize = Linux::getCurrentRSS ();
    stat.startPeakSize    = Linux::getMaxRSS ();
  }

  void end (const std::string &key) {
    VLOG ("End key: " + key);
    Stat &stat = _key_to_stat [key];
    assert (stat.started);
    stat.started = false;
    updateStat (stat);
  }

  std::string toString () {
    std::stringstream ss;

    ss << "--------Memory Statistic--------\n";
    for (auto &kv : _key_to_stat) {
      Stat &s = kv.second;
      ss << "    " << kv.first << "\n";
      ss << "        Count: " << s.count << "\n";
      ss << "        Max current size increase: " << s.maxCurrentSizeInc << "\n";
      ss << "        Max peak size increase: " << s.maxPeakSizeInc << "\n";
      ss << "        Max current size: " << s.maxCurrentSize << "\n";
      ss << "        Max peak size: " << s.maxPeakSize << "\n\n";
    }
    return ss.str ();
  }
};
IStatPtr MemoryStat::instance;

class TimeStat : public IStat {
  struct Stat {
    Stat () : count (0), maxTime (0), totalTime (0), started (false), startTime (0) {}

    /* Stat */
    uint32_t count;
    uint32_t maxTime;
    uint32_t totalTime;

    /* internal */
    bool started;
    std::chrono::milliseconds startTime;    
  };

  static IStatPtr instance;
  std::map <std::string, Stat> _key_to_stat;

  void updateStat (Stat &stat) {
    using namespace std::chrono;
    milliseconds endTime =
      duration_cast<milliseconds> (system_clock::now().time_since_epoch());

    uint32_t time = endTime.count () - stat.startTime.count ();
    //uint32_t t = endTime.count () - stat.startTime.count ();
    if (stat.maxTime < time)
      stat.maxTime = time;

    stat.totalTime += time;
  }

public:
  static IStatPtr getInstance () {
    if (!instance)
      instance.reset (new TimeStat);
    return instance;
  }
  
  void start (const std::string &key) {
    using namespace std::chrono;
    if (_key_to_stat.find (key) == _key_to_stat.end ())
      _key_to_stat [key] = Stat ();

    Stat &stat = _key_to_stat [key];
    assert (!stat.started);
    stat.started = true;
    ++stat.count;
    stat.startTime =
      duration_cast<milliseconds> (system_clock::now().time_since_epoch());
  }

  void end (const std::string &key) {
    Stat &stat = _key_to_stat [key];
    assert (stat.started);
    stat.started = false;
    updateStat (stat);
  }

  std::string toString () {

    std::stringstream ss;

    ss << "--------Time Statistic--------\n";
    for (auto &kv : _key_to_stat) {
      Stat &s = kv.second;
      ss << "    " << kv.first << "\n";
      ss << "        Count: " << s.count << "\n";
      ss << "        Max time: " << s.maxTime << " ms\n";
      ss << "        Total time: " << s.totalTime << " ms\n";
      ss << "        Average time: " << (s.totalTime/s.count) << " ms\n\n";
    }
    return ss.str ();
  }    

};
IStatPtr TimeStat::instance;

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
