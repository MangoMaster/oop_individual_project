#ifndef MIXER_H_
#define MIXER_H_

#include <string>
#include "droplet.h"

namespace DMFB
{
/**
 * @brief Mixer (mixedDroplet1, mixedDroplet2, mixLength, mixWidth, mixTime, name) is-a droplet
 * 
 */
class Mixer : public Droplet
{
  public:
    Mixer(Droplet &d1, Droplet &d2, int l, int w, int t, const std::string &m = "")
        : drop1(d1), drop2(d2), mixLength(l), mixWidth(w), mixTime(t), Droplet(m){};
    ~Mixer(){};
    Mixer &operator=(const Mixer &right)
    {
        this->drop1 = right.drop1;
        this->drop2 = right.drop2;
        this->mixTime - right.mixTime;
        this->sequenceNumMixer = right.sequenceNumMixer;
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
    void getMixSize(int &l, int &w) const
    {
        l = mixLength;
        w = mixWidth;
    }
    int getMixTime() const
    {
        return mixTime;
    }

    /***************************setter***********************/
    void setMixSize(int l, int w)
    {
        mixLength = l;
        mixWidth = w;
    }
    void setMixTime(int t)
    {
        mixTime = t;
    }

    /*************************friend***********************/
    friend class Profile;

  private:
    Droplet &drop1; // 为什么不能用const
    Droplet &drop2;
    int mixLength; // the length of mixer
    int mixWidth;  // the width of mixer
    int mixTime;
    int sequenceNumMixer = -1; // used by class Profile to identify the droplet produced by this mixer.
                                    // sequenceNum identify its number in dropletVec
                                    // sequenceNumMixer identify its number in dropletVec
};

}; // namespace DMFB

#endif // MIXER_H_