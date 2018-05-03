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
    Droplet(const std::string &m = "", int n = 1, int t = 0)
        : name(m), number(n), spawnTime(t), sequenceNumber(sequence)
    {
        assert(n >= 1);
        assert(t >= 0);
        ++sequence;
    }
    Droplet(int n) : name(""), number(n), spawnTime(0)
    {
        assert(n >= 1);
    }
    ~Droplet(){};
    Droplet(const Droplet &d) 
        : name(d.name), number(d.number), spawnTime(d.spawnTime), sequenceNumber(d.sequenceNumber)
    {
        ++sequence;
    }

    /*********************getter**********************/
    int getSequenceNumber() const
    {
        return this->sequenceNumber;
    }
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
    int sequenceNumber;
    std::string name;
    int number;
    int spawnTime;
    static int sequence;

};

}; // namespace DMFB

#endif // DROPLET_H_