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
    Detector(Droplet &d, int t = 0)
        : drop(d), detectTime(t){};
    ~Detector(){};
    Detector &operator=(const Detector &right)
    {
        this->drop = right.drop;
        this->detectTime = right.detectTime;
        this->sequenceNum = right.sequenceNum;
        return *this;
    }

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
    Droplet &drop; // 为什么不能用const
    int detectTime;
    int sequenceNum = -1; // used by class Profile to identify
};

} // namespace DMFB

#endif // DETECTOR_H_