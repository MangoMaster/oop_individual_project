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
    Detector(const Droplet &d, int t = 0)
        : drop(d), detectTime(t){};
    ~Detector(){};  

    /**************************getter************************/
    const std::string &getName() const
    {
        return this->drop.getName();
    }
    int getDetectTime() const
    {
        return this->detectTime;
    }

    /***************************setter*********************/
    void setDetectTime(int t)
    {
        detectTime = t;
    }

  private:
    Droplet drop; // 为什么不能用const？
    int detectTime;
};
}; // namespace DMFB

#endif // DETECTOR_H_