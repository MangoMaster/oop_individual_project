#ifndef DISPENSER_H_
#define DISPENSER_H_

#include <vector>
#include "droplet.h"

namespace DMFB
{
/**
 * @brief Dispenser (dispensedDropVec, spawnTime)
 * 
 */
class Dispenser
{
  public:
    /********************constructor and destructor********************/
    Dispenser(std::vector<Droplet> &d, int t = 0)
        : drops(d), spawnTime(t){};
    ~Dispenser(){};
    Dispenser &operator=(const Dispenser &right)
    {
        this->drops = right.drops;
        this->spawnTime = right.spawnTime;
        this->sequenceNum = right.sequenceNum;
        return *this;
    }

    /**************************getter************************/
    const std::vector<Droplet> &getDropletVec() const
    {
        return this->drops;
    }
    int getSpawnTime() const
    {
        return this->spawnTime;
    }

    /***************************setter*********************/
    void setSpawnTime(int t)
    {
        spawnTime = t;
    }

    /**********************friend***********************/
    friend class Profile;

  private:
    std::vector<Droplet> &drops; // 为什么不能用const
    int spawnTime;
    int sequenceNum = -1; // used by class Profile to identify
};

} // namespace DMFB

#endif // DISPENSER_H_