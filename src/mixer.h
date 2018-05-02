#ifndef MIXER_H_
#define MIXER_H_

#include "droplet.h"

namespace DMFB
{
/**
 * @brief Mixer (mixedDroplet1, mixedDroplet2, mixTime, dropletSelf) is-a Droplet
 * 
 */
class Mixer : public Droplet
{
  public:
    Mixer(){};
    Mixer(const Droplet &d1, const Droplet &d2, int t1, const std::string &m = "", int n = 1, int t2 = 0)
        : drop1(d1), drop2(d2), mixTime(t1), Droplet(m, n, t2){};
    Mixer(const Droplet &d1, const Droplet &d2, int t1, int n)
        : drop1(d1), drop2(d2), mixTime(t1), Droplet(n){};
    ~Mixer(){};

    void getDroplet(Droplet &d1, Droplet &d2)
    {
        d1 = drop1;
        d2 = drop2;
    }
    int getMixTime()
    {
        return mixTime;
    }

    void setDroplet(const Droplet &d1, const Droplet &d2)
    {
        drop1 = d1;
        drop2 = d2;
    }
    void setMixTime(int t)
    {
        mixTime = t;
    }

  private:
    Droplet drop1;
    Droplet drop2;
    int mixTime;
};
}; // namespace DMFB

#endif // MIXER_H_