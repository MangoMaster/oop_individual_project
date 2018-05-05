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
    Mixer(const Droplet &d1, const Droplet &d2, int t1, const std::string &m = "")
        : drop1(d1), drop2(d2), mixTime(t1), Droplet(m), sequenceNumProducedDroplet(-1){};
    ~Mixer(){};

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
    const Droplet &drop1; // 为什么不能用const
    const Droplet &drop2;
    int mixTime;
    int sequenceNumProducedDroplet; // used by class Profile to identify the droplet produced by this mixer.
                                    // sequenceNum identify its number in the vector (dropletVec or mixerVec)
                                    // sequenceNumProducedDroplet identify its number in dropletVec
};

}; // namespace DMFB

#endif // MIXER_H_