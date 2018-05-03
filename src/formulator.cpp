#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "solver.h"
#include "z3++.h"

using namespace std;
using namespace z3;

namespace DMFB
{
Solver::Formulator::Formulator(const Profile &p) : pf(p), cxt()
{
    pf.getSize(xNum, yNum);
}

void Solver::Formulator::formulate()
{
    formulateDroplet();
    formulateDetector();
    formulateDispenser();
    formulateSinker();
}

void Solver::Formulator::formulateDroplet()
{
    z3::expr_vector droplet(cxt);
    for (int i = 0; i < pf.getDropletNum(); ++i)
            for (int j = 0; j < xNum; ++j)
                for (int k = 0; k < yNum; ++k)
                    for (int l = 0; l < pf.getTime(); ++l)
                    {
                        stringstream ss;
                        ss << "droplet:number_" << i
                           << "_position_" << j << "_" << k
                           << "_time_" << l;
                        expr tempExpr = cxt.int_const(ss.str().c_str());
                        droplet.push_back(tempExpr);
                    }
    for (int i = 0; i < pf.getMixerNum(); ++i)
            for (int j = 0; j < xNum; ++j)
                for (int k = 0; k < yNum; ++k)
                    for (int l = 0; l < pf.getTime(); ++l)
                    {
                        stringstream ss;
                        ss << "mixer:number_" << i
                           << "_position_" << j << "_" << k
                           << "_time_" << l;
                        expr tempExpr = cxt.int_const(ss.str().c_str());
                        droplet.push_back(tempExpr);
                    }

    exprVec.push_back(droplet);
}

void Solver::Formulator::formulateDetector()
{
    z3::expr_vector detector(cxt);
    for (int i = 0; i < pf.getDetectorVec().size(); ++i)
        for (int j = 0; j < xNum; ++j)
            for (int k = 0; k < yNum; ++k)
            {
                stringstream ss;
                ss << "detector:number_" << i
                   << "_position_" << j << "_" << k;
                expr tempExpr = cxt.int_const(ss.str().c_str());
                detector.push_back(tempExpr);
            }

    exprVec.push_back(detector);
}

void Solver::Formulator::formulateDispenser()
{
    z3::expr_vector dispenser(cxt);
    for (int i = 0; i < pf.getDropletVec().size(); ++i)
    {
        for (int j = 0; j < pf.getEdgeNum(); ++j)
        {
            stringstream ss;
            ss << "dispenser:net_" << i << "_edge_" << j;
            expr tempExpr = cxt.int_const(ss.str().c_str());
            dispenser.push_back(tempExpr);
        }
    }

    exprVec.push_back(dispenser);
}

void Solver::Formulator::formulateSinker()
{
    z3::expr_vector sinker(cxt);
    for (int i = 0; i < pf.getEdgeNum(); ++i)
    {
        stringstream ss;
        ss << "sinker:edge_" << i;
        expr tempExpr = cxt.int_const(ss.str().c_str());
        sinker.push_back(tempExpr);
    }

    exprVec.push_back(sinker);
}

const void Solver::Formulator::computeDroplet(int &dimension1, int &dimension2, int droplet, int x, int y, int t) const
{
    dimension1 = 0;
    assert(droplet >= 0 && droplet < pf.getDropletNum() + pf.getMixerNum());
    assert(x >= 0 && x < xNum);
    assert(y >= 0 && y < yNum);
    assert(t >= 0 && t < pf.getTime());
    dimension2 = droplet * xNum * yNum * pf.getTime() + x * yNum * pf.getTime() + y * pf.getTime() + t;
}

const void Solver::Formulator::computeDroplet(int &dimension1, std::vector<int> &dimension2, int net, int x, int y, int t) const
{
    dimension1 = 0;
    assert(net >= 0 && net < pf.getDropletVec().size() + pf.getMixerVec().size());
    assert(x >= 0 && x < xNum);
    assert(y >= 0 && y < yNum);
    assert(t >= 0 && t < pf.getTime());
    int dimension2First = 0;
    for (int i = 0; i < net; ++i)
        dimension2First += pf.getDropletVec()[i].getNumber() * xNum * yNum * pf.getTime();
    for (int i = 0; i < pf.getDropletVec()[net].getNumber(); ++i)
        dimension2.push_back(dimension2First + i * xNum * yNum * pf.getTime());
}

const void Solver::Formulator::computeDetector(int &dimension1, int &dimension2, int n, int x, int y) const
{
    dimension1 = 1;
    assert(n >= 0 && n < pf.getDetectorVec().size());
    assert(x >= 0 && x < xNum);
    assert(y >= 0 && y < yNum);
    dimension2 = n * xNum * yNum + x * yNum + y;
}

const void Solver::Formulator::computeDispenser(int &dimension1, int &dimension2, int net, int edge) const
{
    dimension1 = 2;
    assert(net >= 0 && net < pf.getDropletVec().size());
    assert(edge >= 0 && edge < pf.getEdgeNum());
    dimension2 = net * pf.getEdgeNum() + edge;
}
const void Solver::Formulator::computeSinker(int &dimension1, int &dimension2, int edge) const
{
    dimension1 = 3;
    assert(edge >= 0 && edge < pf.getEdgeNum());
    dimension2 = edge;
}

const bool Solver::Formulator::computeAroundChip(int x, int y, vector<int> edge) const
{
    // 从原点开始顺时针排序
    assert(x >= 0 && x < xNum && y >= 0 && y < yNum);
    bool aroundChip = false;
    if (x == 0)
        edge.push_back(y);
    else if (x == xNum - 1)
        edge.push_back(yNum + xNum + yNum - 1 - y);
    // 注意不是else if
    if (y == yNum - 1)
        edge.push_back(yNum + x);
    else if (y == 0)
        edge.push_back(yNum + xNum + yNum + xNum - 1 - x);
    if (edge.empty())
        return false;
    else
        return true;
}

} // namespace DMFB