#ifndef DISPENSER_H_
#define DISPENSER_H_

#include "object.h"
#include "droplet.h"

namespace DMFB
{
/**
 * @brief Dispenser (dispensedDrop, numberOfDispenser, spawnTime)
 * 
 */
class Dispenser : public Object
{
  public:
    /********************constructor and destructor********************/
    Dispenser(const Droplet &d, int n = 1, int t = 0)
        : drop(d), number(n), spawnTime(t){};
    ~Dispenser(){};

    /**************************getter************************/
    const std::string &getName() const
    {
        return this->drop.getName();
    }
    int getNumber() const
    {
        return this->number;
    }
    int getSpawnTime() const
    {
        return this->spawnTime;
    }

    /***************************setter*********************/
    void setNumber(int n)
    {
        number = n;
    }
    void setSpawnTime(int t)
    {
        spawnTime = t;
    }

  private:
    Droplet drop; // 为什么不能用const？
    int number;
    int spawnTime;
};

} // namespace DMFB

#endif // DISPENSER_H_