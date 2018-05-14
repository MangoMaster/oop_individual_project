#ifndef TIMER_H_
#define TIMER_H_

#include <string>
#include <chrono>

namespace DMFB
{
class Timer
{
  public:
    /*****************constructor and destructor*******************/
    Timer(){};
    ~Timer(){};

    /***************************getter****************************/
    double getDuration() const;

    /*************************setter**************************/
    void setStartTime();
    void setEndTime();

    void print(const std::string &printFile) const;

  private:
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
};

} // namespace DMFB

#endif // TIMER_H_