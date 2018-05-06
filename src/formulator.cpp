#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include "formulator.h"
#include "z3++.h"

using namespace std;
using namespace z3;

namespace DMFB
{
Formulator::Formulator(const Profile &p, z3::context &c, z3::expr_vector &ev)
    : pf(p), cxt(c), exprVec(ev)
{
    while (!exprVec.empty())
        exprVec.pop_back();
}

void Formulator::formulate()
{
    formulateDroplet();
    formulateMixer();
    formulateDetector();
    formulateDispenser();
    formulateSinker();
    formulateDetecting();
}

void Formulator::formulateDroplet()
{
    dropletStartSequenceNum = exprVec.size();
    for (int i = 0; i < pf.getDropletNum(); ++i)
        for (int j = 0; j < pf.getSize(); ++j)
            for (int k = 0; k < pf.getTime(); ++k)
            {
                stringstream ss;
                ss << "droplet:number_" << i
                   << "_position_" << j
                   << "_time_" << k;
                // use int_const instead of bool_const for convenience
                exprVec.push_back(cxt.int_const(ss.str().c_str()));
            }
}

void Formulator::formulateMixer()
{
    mixerStartSequenceNum = exprVec.size();
    for (int i = 0; i < pf.getMixerNum(); ++i)
        for (int j = 0; j < pf.getSize(); ++j)
            for (int k = 0; k < pf.getTime(); ++k)
            {
                stringstream ss;
                ss << "mixer:number_" << i
                   << "_position_" << j
                   << "_time<" << k;
                exprVec.push_back(cxt.int_const(ss.str().c_str()));
            }
}

void Formulator::formulateDetector()
{
    detectorStartSequenceNum = exprVec.size();
    for (int i = 0; i < pf.getDetectorNum(); ++i)
        for (int j = 0; j < pf.getSize(); ++j)
        {
            stringstream ss;
            ss << "detector:number_" << i
               << "_position_" << j;
            exprVec.push_back(cxt.int_const(ss.str().c_str()));
        }
}

void Formulator::formulateDispenser()
{
    dispenserStartSequenceNum = exprVec.size();
    for (int i = 0; i < pf.getDispenserNum(); ++i)
        for (int j = 0; j < pf.getEdgeNum(); ++j)
        {
            stringstream ss;
            ss << "dispenser:number_" << i << "_edge_" << j;
            exprVec.push_back(cxt.int_const(ss.str().c_str()));
        }
}

void Formulator::formulateSinker()
{
    sinkerStartSequenceNum = exprVec.size();
    for (int i = 0; i < pf.getEdgeNum(); ++i)
    {
        stringstream ss;
        ss << "sinker:edge_" << i;
        exprVec.push_back(cxt.int_const(ss.str().c_str()));
    }
}

void Formulator::formulateDetecting()
{
    detectingStartSequenceNum = exprVec.size();
    for (int i = 0; i < pf.getDropletNum(); ++i)
        for (int j = 0; j < pf.getTime(); ++j)
        {
            stringstream ss;
            ss << "detecting:droplet_" << i
               << "_time_" << j;
            exprVec.push_back(cxt.int_const(ss.str().c_str()));
        }
}

int Formulator::computeDroplet(int dropletSequenceNum, int position, int time) const
{
    assert(dropletSequenceNum >= 0 && dropletSequenceNum < pf.getDropletNum());
    assert(position >= 0 && position < pf.getSize());
    assert(time >= 0 && time < pf.getTime());
    return dropletStartSequenceNum + dropletSequenceNum * pf.getSize() * pf.getTime() + position * pf.getTime() + time;
}

int Formulator::computeMixer(int mixerSequenceNum, int position, int time) const
{
    assert(mixerSequenceNum >= 0 && mixerSequenceNum < pf.getMixerNum());
    assert(position >= 0 && position < pf.getSize());
    assert(time >= 0 && time < pf.getTime());
    return mixerStartSequenceNum + mixerSequenceNum * pf.getSize() * pf.getTime() + position * pf.getTime() + time;
}

int Formulator::computeDetector(int detectorSequenceNum, int position) const
{
    assert(detectorSequenceNum >= 0 && detectorSequenceNum < pf.getDetectorNum());
    assert(position >= 0 && position < pf.getSize());
    return detectorStartSequenceNum + detectorSequenceNum * pf.getSize() + position;
}

int Formulator::computeDispenser(int dispenserSequenceNum, int edge) const
{
    assert(dispenserSequenceNum >= 0 && dispenserSequenceNum < pf.getDispenserNum());
    assert(edge >= 0 && edge < pf.getEdgeNum());
    return dispenserStartSequenceNum + dispenserSequenceNum * pf.getEdgeNum() + edge;
}

int Formulator::computeSinker(int edge) const
{
    assert(edge >= 0 && edge < pf.getEdgeNum());
    return sinkerStartSequenceNum + edge;
}

int Formulator::computeDetecting(int dropletSequenceNum, int time) const
{
    assert(dropletSequenceNum >= 0 && dropletSequenceNum < pf.getDropletNum());
    assert(time >= 0 && time < pf.getTime());
    return detectingStartSequenceNum + dropletSequenceNum * pf.getTime() + time;
}

} // namespace DMFB