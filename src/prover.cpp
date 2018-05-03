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
            for (int n = 0; n < pf.getDetectorVec().size(); ++n)
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
        for (int n = 0; n < pf.getDropletVec().size(); ++n)
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
                        // 不去心
                        int dimension12, dimension22;
                        formu.computeDroplet(dimension12, dimension22, d2, x, y, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[dimension12][dimension22];
                    }
                    int dimension1, dimension2;
                    formu.computeDroplet(dimension1, dimension2, d, x, y, t);
                    solv.add(implies(exprVec[dimension1][dimension2] == 1, exprNeighbor == 0));
                }

    // N-5 neighbor
}

void Solver::Prover::enoughNumber()
{
    // detector
    for (int det = 0; det < pf.getDetectorVec().size(); ++det)
    {
        expr exprEnough = cxt.int_val(0);
        for (int x = 0; x < xNum; ++x)
            for (int y = 0; y < yNum; ++y)
            {
                int dimension1, dimension2;
                //formu.computeDetector(dimension1, dimension2, )
            }
    }
}
void Solver::Prover::operationMovement() {}
void Solver::Prover::operationMixing() {}
void Solver::Prover::operationDetecting() {}
void Solver::Prover::meetObjective() {}

} // namespace DMFB