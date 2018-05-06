#ifndef MIXER_H_
#define MIXER_H_

#include <string>
#include "droplet.h"

namespace DMFB
{
/**
 * @brief Mixer (mixedDroplet1, mixedDroplet2, mixTime, name) is-a droplet
 * 
 */
class Mixer : public Droplet
{
  public:
    Mixer(Droplet &d1, Droplet &d2, int t1, const std::string &m = "")
        : drop1(d1), drop2(d2), mixTime(t1), Droplet(m), sequenceNumProducedDroplet(-1){};
    ~Mixer(){};
    Mixer &operator=(const Mixer &right)
    {
        this->drop1 = right.drop1;
        this->drop2 = right.drop2;
        this->mixTime - right.mixTime;
        this->sequenceNumProducedDroplet = right.sequenceNumProducedDroplet;
        return *this;
    }

    /************************getter*************************/
    const Droplet &getDroplet1() const
    {
        return drop1;
    }
    const Droplet &getDroplet2() const
    {
        return drop2;
    }
    int getMixTime() const
    {
        return mixTime;
    }

    /***************************setter***********************/
    void setMixTime(int t)
    {
        mixTime = t;
    }

    /*************************friend***********************/
    friend class Profile;

  private:
    Droplet &drop1; // 为什么不能用const
    Droplet &drop2;
    int mixTime;
    int sequenceNumProducedDroplet; // used by class Profile to identify the droplet produced by this mixer.
                                    // sequenceNum identify its number in the vector (dropletVec or mixerVec)
                                    // sequenceNumProducedDroplet identify its number in dropletVec
};

}; // namespace DMFB

#endif // MIXER_H_