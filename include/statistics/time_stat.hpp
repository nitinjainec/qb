#ifndef __TIME_STAT_HPP__
#define __TIME_STAT_HPP__

#include <assert.h>
#include <chrono>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <sys/resource.h>

#include "interface.hpp"
#include "logger.hpp"
#include <util/util.hpp>


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

#endif
