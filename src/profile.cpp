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
    m.sequenceNum = dropletVec.size();
    dropletVec.push_back(m); // object slicing
    // use sequenceNumMixer to identify the mixer
    m.sequenceNumMixer = mixerVec.size();
    mixerVec.push_back(m);
}

void Profile::addMixer(Droplet &d1, Droplet &d2, int l, int w, int t, const std::string &m /* = ""*/)
{
    Mixer mix(d1, d2, l, w, t, m);
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

void Profile::addDetector(Droplet &d, int t)
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

void Profile::addDispenser(vector<Droplet> &d, int t /* = 0*/)
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

// given mixer, find the two droplets that produce it
// given sequenceNum in mixerVec, return two sequenceNum in dropletVec
void Profile::findDropletOfMixer(int mixerSequenceNum, int &dropletSequenceNum1, int &dropletSequenceNum2) const
{
    assert(mixerSequenceNum >= 0 && mixerSequenceNum < getMixerNum());
    dropletSequenceNum1 = mixerVec[mixerSequenceNum].getDroplet1().sequenceNum;
    dropletSequenceNum2 = mixerVec[mixerSequenceNum].getDroplet2().sequenceNum;
}

// given droplet, find the mixer that produces it (if existed)
// given sequenceNum in dropletVec, return sequenceNum in mixerVec or -1
int Profile::findMixerAsDroplet(int dropletSequenceNum) const
{
    for (int i = 0; i < mixerVec.size(); ++i)
        if (mixerVec[i].sequenceNum == dropletSequenceNum)
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

void Profile::computeAroundChip(int position, std::vector<int> &edge) const
{
    assert(position >= 0 && position < getSize());
    // position 从左上角开始，一行一行排序
    // edge 从左上角开始，顺时针排序
    if (position < length) // 上方一行edge
        edge.push_back(position);
    for (int y = 0; y < width; ++y)
        if (position == y * length + length - 1) // 右方一列edge
            edge.push_back(length + y);
    // 注意是if不是else if
    if (position >= length * width - length) // 下方一行edge
        edge.push_back(length + width + length * width - 1 - position);
    for (int y = 0; y < width; ++y)
        if (position == y * length) // 左方一列edge
            edge.push_back(length + width + length + width - y - 1);
}

void Profile::computeGraph(int mixerSequenceNum, int position, std::vector<std::vector<int> > &graph) const
{
    int x, y;
    computeXY(x, y, position);
    int xNum, yNum;
    getSize(xNum, yNum);
    int lengthMixer, widthMixer;
    mixerVec[mixerSequenceNum].getMixSize(lengthMixer, widthMixer);

    for (int i = 0; i < lengthMixer; ++i)
        for (int j = 0; j < widthMixer; ++j)
        {
            int xMin = x - i;
            int xMax = x + lengthMixer - 1 - i;
            int yMin = y - j;
            int yMax = y + widthMixer - 1 - j;
            if (xMin < 0 || xMax >= xNum || yMin < 0 || yMax >= yNum)
                continue;
            vector<int> tempGraph;
            for (int ii = xMin; ii <= xMax; ++ii)
                for (int jj = yMin; jj <= yMax; ++jj)
                {
                    int tempPosition = computePosition(ii, jj);
                    tempGraph.push_back(tempPosition);
                }
            graph.push_back(tempGraph);
        }

    for (int i = 0; i < widthMixer; ++i)
        for (int j = 0; j < lengthMixer; ++j)
        {
            int xMin = x - i;
            int xMax = x + widthMixer - 1 - i;
            int yMin = y - j;
            int yMax = y + lengthMixer - 1 - j;
            if (xMin < 0 || xMax >= xNum || yMin < 0 || yMax >= yNum)
                continue;
            vector<int> tempGraph;
            for (int ii = xMin; ii <= xMax; ++ii)
                for (int jj = yMin; jj <= yMax; ++jj)
                {
                    int tempPosition = computePosition(ii, jj);
                    tempGraph.push_back(tempPosition);
                }
            graph.push_back(tempGraph);
        }
}

} // namespace DMFB