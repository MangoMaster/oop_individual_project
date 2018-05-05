#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <string>
#include "droplet.h"

namespace DMFB
{
/**
 * @brief Detector (detectedDrop, detectTime)
 * 
 */
class Detector
{
  public:
    /********************constructor and destructor********************/
    Detector(const Droplet &d, int t = 0)
        : drop(d), detectTime(t), sequenceNum(-1){};
    ~Detector(){};

    /**************************getter************************/
    const Droplet &getDroplet() const
    {
        return this->drop;
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

    /**********************friend*********************/
    friend class Profile;

  private:
    const Droplet &drop; // 为什么不能用const
    int detectTime;
    int sequenceNum; // used by class Profile to identify
};
}; // namespace DMFB

#endif // DETECTOR_H_