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
    netNum = pf.getDropletVec().size() + pf.getMixerVec().size();
    pf.getSize(xNum, yNum);
    tNum = pf.getTime();
    edgeNum = (2 * xNum + 2 * yNum);
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
    for (int i = 0; i < pf.getDropletVec().size(); ++i)
        for (int temp = 0; temp < pf.getDropletVec()[i].getNumber(); ++temp)
            for (int j = 0; j < xNum; ++j)
                for (int k = 0; k < yNum; ++k)
                    for (int l = 0; l < tNum; ++l)
                    {
                        stringstream ss;
                        ss << "droplet:number_" << droplet.size()
                           << "_net_" << i
                           << "_position_" << j << "_" << k
                           << "_time_" << l;
                        expr tempExpr = cxt.bool_const(ss.str().c_str());
                        droplet.push_back(tempExpr);
                    }
    for (int i = 0; i < pf.getMixerVec().size(); ++i)
        for (int temp = 0; temp < pf.getDropletVec()[i].getNumber(); ++temp)
            for (int j = 0; j < xNum; ++j)
                for (int k = 0; k < yNum; ++k)
                    for (int l = 0; l < tNum; ++l)
                    {
                        stringstream ss;
                        ss << "droplet_mixer:number_" << droplet.size()
                           << "_net_" << i
                           << "_position_" << j << "_" << k
                           << "_time_" << l;
                        expr tempExpr = cxt.bool_const(ss.str().c_str());
                        droplet.push_back(tempExpr);
                    }

    exprVec.push_back(droplet);
}

void Solver::Formulator::formulateDetector()
{
    z3::expr_vector detector(cxt);
    for (int i = 0; i < netNum; ++i)
        for (int j = 0; j < xNum; ++j)
            for (int k = 0; k < yNum; ++k)
            {
                stringstream ss;
                ss << "detector:net_" << i
                   << "_position_" << j << "_" << k;
                expr tempExpr = cxt.bool_const(ss.str().c_str());
                detector.push_back(tempExpr);
            }

    exprVec.push_back(detector);
}

void Solver::Formulator::formulateDispenser()
{
    z3::expr_vector dispenser(cxt);
    for (int i = 0; i < edgeNum; ++i)
    {
        stringstream ss;
        ss << "dispenser:net_" << i;
        expr tempExpr = cxt.bool_const(ss.str().c_str());
        dispenser.push_back(tempExpr);
    }

    exprVec.push_back(dispenser);
}

void Solver::Formulator::formulateSinker()
{
    z3::expr_vector sinker(cxt);
    for (int i = 0; i < edgeNum; ++i)
    {
        stringstream ss;
        ss << "sinker:net_" << i;
        expr tempExpr = cxt.bool_const(ss.str().c_str());
        sinker.push_back(tempExpr);
    }

    exprVec.push_back(sinker);
}

const void Solver::Formulator::computeDroplet(int &dimension1, std::vector<int> &dimension2, int net, int x, int y, int time) const
{
    dimension1 = 0;
    assert(net >= 0 && net < netNum);
    assert(x >= 0 && x < xNum);
    assert(y >= 0 && y < yNum);
    assert(time >= 0 && time < tNum);
    int dimension2First = 0;
    for (int i = 0; i < net; ++i)
        dimension2First += pf.getDropletVec()[i].getNumber() * xNum * yNum * tNum;
    for (int i = 0; i < pf.getDropletVec()[net].getNumber(); ++i)
        dimension2.push_back(dimension2First + i * xNum * yNum * tNum);
}

const void Solver::Formulator::computeDetector(int &dimension1, int &dimension2, int net, int x, int y) const
{
    dimension1 = 1;
    assert(net >= 0 && net < netNum);
    assert(x >= 0 && x < xNum);
    assert(y >= 0 && y < yNum);
    dimension2 = net * xNum * yNum + x * yNum + y;
}

const void Solver::Formulator::computeDispenser(int &dimension1, int &dimension2, int xEdge, int yEdge) const
{
    dimension1 = 2;
    assert(xEdge == -1 || xEdge == xNum || yEdge == -1 || yEdge == yNum);
    // 从原点开始顺时针排序
    if (xEdge == -1)
    {
        assert(yEdge >= 0 && yEdge < yNum);
        dimension2 = yEdge;
    }
    else if (yEdge == yNum)
    {
        assert(xEdge >= 0 && xEdge < xNum);
        dimension2 = yNum + xEdge;
    }
    else if (xEdge == xNum)
    {
        assert(yEdge >= 0 && yEdge < yNum);
        dimension2 = yNum + xNum + yNum - 1 - yEdge;
    }
    else if (yEdge == -1)
    {
        assert(xEdge >= 0 && xEdge < xNum);
        dimension2 = yNum + xNum + yNum + xNum - 1 - xEdge;
    }
}
inline const void Solver::Formulator::computeSinker(int &dimension1, int &dimension2, int xEdge, int yEdge) const
{
    dimension1 = 3;
    assert(xEdge == -1 || xEdge == xNum || yEdge == -1 || yEdge == yNum);
    // 从原点开始顺时针排序
    if (xEdge == -1)
    {
        assert(yEdge >= 0 && yEdge < yNum);
        dimension2 = yEdge;
    }
    else if (yEdge == yNum)
    {
        assert(xEdge >= 0 && xEdge < xNum);
        dimension2 = yNum + xEdge;
    }
    else if (xEdge == xNum)
    {
        assert(yEdge >= 0 && yEdge < yNum);
        dimension2 = yNum + xNum + yNum - 1 - yEdge;
    }
    else if (yEdge == -1)
    {
        assert(xEdge >= 0 && xEdge < xNum);
        dimension2 = yNum + xNum + yNum + xNum - 1 - xEdge;
    }
}

} // namespace DMFB