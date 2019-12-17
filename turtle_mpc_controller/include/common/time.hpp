#ifndef TIMES_HPP_
#define TIMES_HPP_

#include <sys/time.h>       /* gettimeofday */
#include <ros/ros.h>        /* ros::Time::now() */
//clock()函数的精确度是10毫秒(ms)
//gettimofday()函数的精确度是微秒(μs)
namespace common {
//----------------------------------- Timer helper class
class Clock {

 public:
  Clock() { start(); }

  /// \brief Start clock timer.
  void start() {
    gettimeofday(&real_time_start_, NULL);
    cpu_start_ = clock();
  }

  /// \brief Sample clock timer.
  void takeTime() {
    //end只是一个对象，不是关键字
    struct timeval end;
    gettimeofday(&end, NULL);
    cpu_time_ms_ = double(clock() - cpu_start_) / CLOCKS_PER_SEC * kSecondsToMiliseconds;

    long seconds, useconds;
    //tv_sec = 1000000 * tv_usec

    seconds = end.tv_sec - real_time_start_.tv_sec;
    useconds = end.tv_usec - real_time_start_.tv_usec;
    real_time_ms_ = (seconds * kSecondsToMiliseconds +
        useconds * kMicrosecondsToMiliseconds) + 0.5;
  }

  /// \brief Return elapsed physical time.
  double getRealTime() { return real_time_ms_; }

  /// \brief Return elapsed CPU time.
  double getCPUTime() { return cpu_time_ms_; }

  double takeRealTime() {
    takeTime();
    return getRealTime();
  }

 private:
  struct timeval real_time_start_;
  double real_time_ms_, cpu_time_ms_;
  clock_t cpu_start_;

  static constexpr double kSecondsToMiliseconds = 1000.0;
  static constexpr double kMicrosecondsToMiliseconds = 0.001;
};

static std::string getCurrentTimestampString() {
  long long current_time = ros::Time::now().toSec() * 100;
  return std::to_string(current_time);
}
}

#endif /* TIMES_HPP_ */