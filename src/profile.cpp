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


} // namespace DMFB