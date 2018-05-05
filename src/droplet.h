#ifndef DROPLET_H_
#define DROPLET_H_

#include <string>
#include <cassert>
#include "object.h"

namespace DMFB
{
/**
 * @brief Droplet (name, number, spawnTime)
 * 
 */
class Droplet : public Object
{
  public:
    /*****************constructor and destructor********************/
    Droplet(const std::string &m, int n = 1, int t = 0)
        : name(m), number(n), spawnTime(t)
    {
        assert(n >= 1);
        assert(t >= 0);
    }
    ~Droplet(){};

    /*********************getter**********************/
    int getNumber() const
    {
        return this->number;
    }
    const std::string &getName() const
    {
        return this->name;
    }
    int getSpawnTime() const
    {
        return this->spawnTime;
    }

    /*********************setter**********************/
    void setName(const std::string &m)
    {
        name = m;
    }
    void setNumber(int n)
    {
        assert(n >= 1);
        number = n;
    }
    void addNumber(int n)
    {
        number += n;
    }
    void subtractNumber(int n)
    {
        assert(number > n);
        number -= n;
    }
    void setSpawnTime(int t)
    {
        assert(t >= 0);
        spawnTime = t;
    }

  private:
    std::string name;
    int number;
    int spawnTime;

};

}; // namespace DMFB

#endif // DROPLET_H_