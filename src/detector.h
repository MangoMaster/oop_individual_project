#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <string>
#include "object.h"
#include "droplet.h"

namespace DMFB
{
/**
 * @brief Detector (detectedDrop, detectTime)
 * 
 */
class Detector : public Object
{
  public:
    /********************constructor and destructor********************/
    Detector(const Droplet &d, int n = 0, int t = 0)
        : drop(d), number(n), detectTime(t){};
    ~Detector(){};  

    /**************************getter************************/
    const std::string &getName() const
    {
        return this->drop.getName();
    }
    int getNumber() const
    {
        return this->number;
    }
    int getDetectTime() const
    {
        return this->detectTime;
    }

    /***************************setter*********************/
    void setNumber(int n)
    {
        number = n;
    }
    void setDetectTime(int t)
    {
        detectTime = t;
    }

  private:
    Droplet drop; // 为什么不能用const？
    int number;
    int detectTime;
};
}; // namespace DMFB

#endif // DETECTOR_H_