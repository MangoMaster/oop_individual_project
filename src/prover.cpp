#include <iostream>
#include <vector>
#include <cassert>
#include "solver.h"

using namespace std;
using namespace z3;

namespace DMFB
{

Solver::Prover::Prover(const Profile &p, const Formulator &f)
    : pf(p), formu(f), cxt(f.getContext()), solv(f.getContext()), exprVec(f.getExprVector())
{
    pf.getSize(xNum, yNum);
}

void Solver::Prover::prove()
{
    //TODO:
}

void Solver::Prover::isBool()
{
    // 0-false  1-true
    for (int i = 0; i < exprVec.size(); ++i)
        for (int j = 0; j < exprVec[i].size(); ++j)
            solv.add(exprVec[i][j] == 0 || exprVec[i][j] == 1);
}

void Solver::Prover::noSpaceOverlap()
{
    // droplet
    for (int x = 0; x < xNum; ++x)
        for (int y = 0; y < yNum; ++y)
            for (int t = 0; t < pf.getTime(); ++t)
            {
                expr exprDroplet = cxt.int_val(0);
                for (int n = 0; n < pf.getDropletNum() + pf.getMixerNum(); ++n)
                {
                    int dimension1, dimension2;
                    formu.computeDroplet(dimension1, dimension2, n, x, y, t);
                    exprDroplet = exprDroplet + exprVec[dimension1][dimension2];
                }
                solv.add(exprDroplet <= 1);
            }

    // detector
    for (int x = 0; x < xNum; ++x)
        for (int y = 0; y < yNum; ++y)
        {
            expr exprDetector = cxt.int_val(0);
            for (int n = 0; n < pf.getDetectorNum(); ++n)
            {
                int dimension1, dimension2;
                formu.computeDetector(dimension1, dimension2, n, x, y);
                exprDetector = exprDetector + exprVec[dimension1][dimension2];
            }
            solv.add(exprDetector <= 1);
        }

    // dispenser & sinker
    for (int e = 0; e < pf.getEdgeNum(); ++e)
    {
        expr exprDispenserSinker = cxt.int_val(0);
        for (int n = 0; n < pf.getDispenserNum(); ++n)
        {
            int dimension1, dimension2;
            formu.computeDispenser(dimension1, dimension2, n, e);
            exprDispenserSinker = exprDispenserSinker + exprVec[dimension1][dimension2];
        }
        int dimension1, dimension2;
        formu.computeSinker(dimension1, dimension2, e);
        exprDispenserSinker = exprDispenserSinker + exprVec[dimension1][dimension2];
        solv.add(exprDispenserSinker <= 1);
    }
}

void Solver::Prover::noTimeOverlap()
{
    //droplet (not mixer)
    for (int n = 0; n < pf.getDropletNum(); ++n)
        for (int t = 0; t < pf.getTime(); ++t)
        {
            expr exprDroplet = cxt.int_val(0);
            for (int x = 0; x < xNum; ++x)
                for (int y = 0; y < yNum; ++y)
                {
                    int dimension1, dimension2;
                    formu.computeDroplet(dimension1, dimension2, n, x, y, t);
                    exprDroplet = exprDroplet + exprVec[dimension1][dimension2];
                }
            solv.add(exprDroplet <= 1);
        }
}

void Solver::Prover::noSpaceNeighbor()
{
    // N-9 neighbor
    for (int d = 0; d < pf.getDropletNum() + pf.getMixerNum(); ++d)
        for (int x = 0; x < xNum; ++x)
            for (int y = 0; y < yNum; ++y)
                for (int t = 0; t < pf.getTime(); ++t)
                {
                    expr exprNeighbor = cxt.int_val(0);
                    for (int d2 = 0; d2 < pf.getDropletNum() + pf.getMixerNum(); ++d2)
                    {
                        if (d2 == d)
                            continue;
                        for (int i = 0; i < sizeof(N9X) / sizeof(N9X[0]); ++i)
                        {
                            int x2 = x + N9X[i];
                            int y2 = y + N9Y[i];
                            if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                                continue;
                            int dimension12, dimension22;
                            formu.computeDroplet(dimension12, dimension22, d2, x2, y2, t);
                            exprNeighbor = exprNeighbor + exprVec[dimension12][dimension22];
                        }
                    }
                    int dimension1, dimension2;
                    formu.computeDroplet(dimension1, dimension2, d, x, y, t);
                    solv.add(implies(exprVec[dimension1][dimension2] == 1, exprNeighbor == 0));
                }

    // N-5 neighbor

    // detector
}

void Solver::Prover::noTimeNeighbor()
{
    // N-9 neighbor
    for (int d = 0; d < pf.getDropletNum() + pf.getMixerNum(); ++d)
        for (int x = 0; x < xNum; ++x)
            for (int y = 0; y < yNum; ++y)
                for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1 不存在
                {
                    expr exprNeighbor = cxt.int_val(0);
                    for (int d2 = 0; d2 < pf.getDropletNum() + pf.getMixerNum(); ++d2)
                    {
                        if (d2 == d)
                            continue;
                        for (int i = 0; i < sizeof(N9X) / sizeof(N9X[0]); ++i)
                        {
                            int x2 = x + N9X[i];
                            int y2 = y + N9Y[i];
                            if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                                continue;
                            int dimension12, dimension22;
                            formu.computeDroplet(dimension12, dimension22, d2, x2, y2, t - 1);
                            exprNeighbor = exprNeighbor + exprVec[dimension12][dimension22];
                        }
                        { // 不去心
                            int dimension12, dimension22;
                            formu.computeDroplet(dimension12, dimension22, d2, x, y, t - 1);
                            exprNeighbor = exprNeighbor + exprVec[dimension12][dimension22];
                        }
                    }
                    {
                        int dimension1, dimension2;
                        formu.computeDroplet(dimension1, dimension2, d, x, y, t);
                        solv.add(implies(exprVec[dimension1][dimension2] == 1, exprNeighbor == 0));
                    }
                }

    // N-5 neighbor
}

void Solver::Prover::enoughNumber()
{
    // droplet
    for (int d = 0; d < pf.getDropletNum() + pf.getMixerNum(); ++d)
    {
        expr exprEnough = cxt.int_val(0);
        for (int x = 0; x < xNum; ++x)
            for (int y = 0; y < yNum; ++y)
                for (int t = 0; t < pf.getTime(); ++t)
                {
                    int dimension1, dimension2;
                    formu.computeDroplet(dimension1, dimension2, d, x, y, t);
                    exprEnough = exprEnough + exprVec[dimension1][dimension2];
                }
        solv.add(exprEnough >= 1);
    }

    // detector
    for (int n = 0; n < pf.getDetectorVec().size(); ++n)
    {
        expr exprEnough = cxt.int_val(0);
        for (int x = 0; x < xNum; ++x)
            for (int y = 0; y < yNum; ++y)
            {
                int dimension1, dimension2;
                formu.computeDetector(dimension1, dimension2, n, x, y);
                exprEnough = exprEnough + exprVec[dimension1][dimension2];
            }
        solv.add(exprEnough == pf.getDetectorVec()[n].getNumber()); // 默认detector只有一个，可拓展
    }

    // dispenser
    for (int n = 0; n < pf.getDispenserVec().size(); ++n)
    {
        expr exprEnough = cxt.int_val(0);
        for (int edge = 0; edge < pf.getEdgeNum(); ++edge)
        {
            int dimension1, dimension2;
            formu.computeDispenser(dimension1, dimension2, n, edge);
            exprEnough = exprEnough + exprVec[dimension1][dimension2];
        }
        solv.add(exprEnough == pf.getDispenserVec()[n].getNumber());
    }

    // sinker
    {
        expr exprEnough = cxt.int_val(0);
        for (int edge = 0; edge < pf.getEdgeNum(); ++edge)
        {
            int dimension1, dimension2;
            formu.computeSinker(dimension1, dimension2, edge);
            exprEnough = exprEnough + exprVec[dimension1][dimension2];
        }
        solv.add(exprEnough == pf.getSinkerNum());
    }
}

void Solver::Prover::operationMovement()
{

    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int x = 0; x < xNum; ++x)
            for (int y = 0; y < yNum; ++y)
            {
                expr exprDispenser = cxt.int_val(0);
                {
                    // dispenser produce
                    vector<int> e;
                    formu.computeAroundChip(x, y, e);
                    for (int i = 0; i < e.size(); ++i)
                    {
                        int dimension1, dimension2;
                        formu.computeDispenser(dimension1, dimension2, pf.getDropletNet(d), e[i]);
                        exprDispenser = exprDispenser + exprVec[dimension1][dimension2];
                    }
                }
                for (int t = 1; t < pf.getTime(); ++t)
                {
                    expr exprMovement = cxt.int_val(0);
                    // move from neighbor
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int dimension12, dimension22;
                        formu.computeDroplet(dimension12, dimension22, d, x2, y2, t - 1);
                        exprMovement = exprMovement + exprVec[dimension12][dimension22];
                    }
                    { // 不去心
                        int dimension12, dimension22;
                        formu.computeDroplet(dimension12, dimension22, d, x, y, t - 1);
                        exprMovement = exprMovement + exprVec[dimension12][dimension22];
                    }

                    // mixer produce
                    bool mixerExist = pf.existMixer(d);
                    if (mixerExist)
                    {
                        Mixer mi = pf.getMixer(d);
                        int dimension12, dimension22;
                        formu.computeDroplet(dimension12, dimension22, mi, x, y, t);
                        exprMovement = exprMovement + exprVec[dimension12][dimension22];
                    }

                    {
                        int dimension1, dimension2;
                        formu.computeDroplet(dimension1, dimension2, d, x, y, t);
                        solv.add(implies(exprVec[dimension1][dimension2] == 1, exprMovement == 1) || implies(exprVec[dimension1][dimension2] == 1, exprDispenser >= 1));
                    }
                }
            }
}

void Solver::Prover::operationDisappear()
{
    for (int x = 0; x < xNum; ++x)
        for (int y = 0; y < yNum; ++y)
        {
            expr exprSinker = int_val(0);
            {
                // sinker produce
                vector<int> e;
                formu.computeAroundChip(x, y, e);
                for (int i = 0; i < e.size(); ++i)
                {
                    int dimension1, dimension2;
                    formu.computeDispenser(dimension1, dimension2, pf.getDropletNet(d), e[i]);
                    exprSinker = exprSinker + exprVec[dimension1][dimension2];
                }
            }
            for (int d = 0; d < pf.getDropletNum(); ++d)
                for (int t = 0; t < pf.getTime() - 1; ++t)
                {
                    expr exprNeighbor = cxt.int_val(0);
                    expr expr bool mixerExist = pf.existMixer(d);
                    if (mixerExist)
                    {
                        Mixer mi = pf.getMixer(d);
                    }
                    // neighbor
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int dimension12, dimension22;
                        formu.computeDroplet(dimension12, dimension22, d, x2, y2, t + 1);
                        exprNeighbor = exprNeighbor + exprVec[dimension12][dimension22];
                    }
                    { // 不去心
                        int dimension12, dimension22;
                        formu.computeDroplet(dimension12, dimension22, d, x, y, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[dimension12][dimension22];
                    }
                    {
                        int dimension1, dimension2;
                        formu.computeDroplet(dimension1, dimension2, d, x, y, t);
                        solv.add(implies(exprVec[dimension1][dimension2] == 1 && exprNeighbor == 0, exprSinker == 1) ||
                                 implies(exprVec[dimension1][dimension2] == 1 && exprNeighbor == 0, exprMixer >= 1 && exprMixerPre == 0));
                    }
                }
        }
}

void Solver::Prover::operationMixing() {}
void Solver::Prover::operationDetecting()
{
    for (int n = 0; n < pf.getDetectorNum(); ++n)
        for (int t = 0; t < pf.getTime(); ++t)
        for (int x = 0; x < xNum; ++x)
        for (int y = 0; y < yNum; ++y)
        {
            int dimension1, dimension2;
            formu.computeDetector(dimension1, dimension2, n, x, y);
            solv.add(exprVec)
        }
}

void Solver::Prover::meetObjective() {}

} // namespace DMFB