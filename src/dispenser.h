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
    Dispenser(const std::vector<Droplet> &d, int t = 0)
        : drops(d), spawnTime(t), sequenceNum(-1){};
    ~Dispenser(){};

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
    const std::vector<Droplet> &drops;
    int spawnTime;
    int sequenceNum; // used by class Profile to identify
};

} // namespace DMFB

#endif // DISPENSER_H_