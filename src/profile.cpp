#include <iostream>
#include "profile.h"

using namespace std;

namespace DMFB
{
int Profile::getDropletNum() const
{
    int dropletNum = 0;
    for (int i = 0; i < dropletVec.size(); ++i)
        dropletNum += dropletVec[i].getNumber();
    return dropletNum;
}

int Profile::getMixerNum() const
{
    int mixerNum = 0;
    for (int i = 0; i < mixerVec.size(); ++i)
        mixerNum += mixerVec[i].getNumber();
    return mixerNum;
}

int Profile::getDetectorNum() const
{
    int detectorNum = 0;
    for (int i = 0; i < detectorVec.size(); ++i)        
        detectorNum += detectorVec[i].getNumber();
    return detectorNum;
}

int Profile::getDispenserNum() const
{
    int dispenserNum = 0;
    for (int i = 0; i < dispenserVec.size(); ++i)
        dispenserNum += dispenserVec[i].getNumber();
    return dispenserNum;
}

} // namespace DMFB