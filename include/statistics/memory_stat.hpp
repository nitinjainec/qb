#ifndef __MEMORY_STAT_HPP__
#define __MEMORY_STAT_HPP__

#include <assert.h>
#include <chrono>
#include <sstream>
#include <vector>

#include <interface.hpp>
#include <util/logger.hpp>
#include <util/util.hpp>

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
    size_t currentSize = util::linux::getCurrentRSS ();
    size_t peakSize    = util::linux::getMaxRSS ();

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
    stat.startCurrentSize = util::linux::getCurrentRSS ();
    stat.startPeakSize    = util::linux::getMaxRSS ();
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

#endif
