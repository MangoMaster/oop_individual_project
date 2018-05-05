#include <cassert>
#include "profile.h"

using namespace std;

namespace DMFB
{
void Profile::addDroplet(Droplet &d)
{
    // use sequenceNum to identify the droplet
    d.sequenceNum = dropletVec.size();
    dropletVec.push_back(d);
}

void Profile::addDroplet(const std::string &m /* = ""*/)
{
    Droplet d(m);
    addDroplet(d);
}

void Profile::addMixer(Mixer &m)
{
    // make sure droplets have been added to dropletVec
    assert(m.getDroplet1().sequenceNum >= 0 && m.getDroplet2().sequenceNum >= 0);
    // use sequenceNum to identify the mixer (is-a droplet)
    m.sequenceNumProducedDroplet = m.sequenceNum = dropletVec.size();
    dropletVec.push_back(m); // object slicing
    // use sequenceNum to identify the mixer
    m.sequenceNum = mixerVec.size();
    mixerVec.push_back(m);
}

void Profile::addMixer(const Droplet &d1, const Droplet &d2, int t1, const std::string &m /* = ""*/)
{
    Mixer mix(d1, d2, t1, m);
    addMixer(mix);
}

void Profile::addDetector(Detector &d)
{
    // make sure droplet has been added to dropletVec
    assert(d.getDroplet().sequenceNum >= 0);
    // use sequenceNum to identify the detector
    d.sequenceNum = detectorVec.size();
    detectorVec.push_back(d);
}

void Profile::addDetector(const Droplet &d, int t)
{
    Detector det(d, t);
    addDetector(det);
}

void Profile::addDispenser(Dispenser &d)
{
    // make sure droplet has been added to dropletVec
    for (int i = 0; i < d.getDropletVec().size(); ++i)
        assert(d.getDropletVec()[i].sequenceNum >= 0);
    // use sequenceNum to identify the dispenser
    d.sequenceNum = dispenserVec.size();
    dispenserVec.push_back(d);
}

void Profile::addDispenser(const vector<Droplet> &d, int t = 0)
{
    Dispenser dis(d, t);
    dispenserVec.push_back(dis);
}


// given droplet, find the mixer that will accept it (if existed)
// given sequenceNum in dropletVec, return sequenceNum in mixerVec or -1
int Profile::findMixerOfDroplet(int dropletSequenceNum) const
{
    for (int i = 0; i < mixerVec.size(); ++i)
        if (mixerVec[i].getDroplet1().sequenceNum == dropletSequenceNum || mixerVec[i].getDroplet2().sequenceNum == dropletSequenceNum)
            return i;
    return -1;
}

// given droplet, find the mixer that produces it (if existed)
// given sequenceNum in dropletVec, return sequenceNum in mixerVec or -1
int Profile::findMixerAsDroplet(int dropletSequenceNum) const
{
    for (int i = 0; i < mixerVec.size(); ++i)
        if (mixerVec[i].sequenceNumProducedDroplet == dropletSequenceNum)
            return i;
    return -1;
}

// given droplet, find its detector (if existed)
// given sequenceNum in dropletVec, return sequenceNum in dispenserVec or -1
int Profile::findDetectorOfDroplet(int dropletSequenceNum) const
{
    for (int i = 0; i < detectorVec.size(); ++i)
        if (detectorVec[i].getDroplet().sequenceNum == dropletSequenceNum)
            return i;
    return -1;
}

// given droplet, find its dispenser (if existed)
// given sequenceNum in dropletVec, return sequenceNum in dispenserVec or -1
int Profile::findDispenserOfDroplet(int dropletSequenceNum) const
{
    for (int i = 0; i < dispenserVec.size(); ++i)
        for (int j = 0; j < dispenserVec[i].getDropletVec().size(); ++j)
            if (dispenserVec[i].getDropletVec()[j].sequenceNum == dropletSequenceNum)
                return i;
    return -1;
}
} // namespace DMFB