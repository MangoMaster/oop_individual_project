#include <iostream>
#include <vector>
#include <cassert>
#include "profile.h"
#include "formulator.h"
#include "prover.h"

using namespace std;
using namespace z3;

namespace DMFB
{

const int Prover::N9X[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int Prover::N9Y[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
const int Prover::N5X[4] = {-1, 0, 1, 0};
const int Prover::N5Y[4] = {0, 1, 0, -1};

Prover::Prover(const Profile &p, const z3::expr_vector &ev, const Formulator &f, z3::context &c, z3::solver &s)
    : pf(p), exprVec(ev), formu(f), cxt(c), solv(s)
{
}

void Prover::prove()
{
    //TODO:
}

void Prover::isBool()
{
    // 0-false  1-true
    for (int i = 0; i < exprVec.size(); ++i)
        solv.add(exprVec[i] == 0 || exprVec[i] == 1);
}

void Prover::noSpaceOverlap()
{
    // droplet
    for (int p = 0; p < pf.getSize(); ++p)
        for (int t = 0; t < pf.getTime(); ++t)
        {
            expr exprDroplet = cxt.int_val(0);
            for (int d = 0; d < pf.getDropletNum(); ++d)
            {
                int sequenceNum = formu.computeDroplet(d, p, t);
                exprDroplet = exprDroplet + exprVec[sequenceNum];
            }
            solv.add(exprDroplet <= 1);
        }

    // detector
    for (int p = 0; p < pf.getSize(); ++p)
    {
        expr exprDetector = cxt.int_val(0);
        for (int n = 0; n < pf.getDetectorNum(); ++n)
        {
            int sequenceNum = formu.computeDetector(n, p);
            exprDetector = exprDetector + exprVec[sequenceNum];
        }
        solv.add(exprDetector <= 1);
    }

    // dispenser & sinker
    for (int e = 0; e < pf.getEdgeNum(); ++e)
    {
        expr exprDispenserSinker = cxt.int_val(0);
        for (int n = 0; n < pf.getDispenserNum(); ++n)
        {
            int sequenceNum = formu.computeDispenser(n, e);
            exprDispenserSinker = exprDispenserSinker + exprVec[sequenceNum];
        }
        // sinker
        {
            int sequenceNum = formu.computeSinker(e);
            exprDispenserSinker = exprDispenserSinker + exprVec[sequenceNum];
        }
        solv.add(exprDispenserSinker <= 1);
    }
}

void Prover::noTimeOverlap()
{
    //droplet
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int t = 0; t < pf.getTime(); ++t)
        {
            expr exprDroplet = cxt.int_val(0);
            for (int p = 0; p < pf.getSize(); ++p)
            {
                int sequenceNum = formu.computeDroplet(d, p, t);
                exprDroplet = exprDroplet + exprVec[sequenceNum];
            }
            solv.add(exprDroplet <= 1);
        }
}

void Prover::noN9SpaceNeighbor()
{
    int xNum, yNum;
    pf.getSize(xNum, yNum);
    // N-9 neighbor
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 0; t < pf.getTime(); ++t)
            {
                expr exprNeighbor = cxt.int_val(0);
                for (int d2 = d + 1; d2 < pf.getDropletNum(); ++d2) // 由对称性只需计算一半
                {
                    for (int i = 0; i < sizeof(N9X) / sizeof(N9X[0]); ++i)
                    {
                        int x2 = x + N9X[i];
                        int y2 = y + N9Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeDroplet(d2, p2, t);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                int sequenceNum = formu.computeDroplet(d, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }

    // detector
}

void Prover::noN5SpaceNeighbor()
{
    int xNum, yNum;
    pf.getSize(xNum, yNum);
    // N-5 neighbor
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 0; t < pf.getTime(); ++t)
            {
                expr exprNeighbor = cxt.int_val(0);
                for (int d2 = d + 1; d2 < pf.getDropletNum(); ++d2) // 由对称性只需计算一半
                {
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeDroplet(d2, p2, t);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                int sequenceNum = formu.computeDroplet(d, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }
}

void Prover::noN9TimeNeighbor()
{
    int xNum, yNum;
    pf.getSize(xNum, yNum);
    // N-9 neighbor
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1 不存在
            {
                expr exprNeighbor = cxt.int_val(0);
                for (int d2 = 0; d2 < pf.getDropletNum(); ++d2) // 无对称性，必须都算
                {
                    if (d2 == d)
                        continue;
                    for (int i = 0; i < sizeof(N9X) / sizeof(N9X[0]); ++i)
                    {
                        int x2 = x + N9X[i];
                        int y2 = y + N9Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeDroplet(d2, p2, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                    { // 不去心
                        int sequenceNum2 = formu.computeDroplet(d2, p, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                {
                    int sequenceNum = formu.computeDroplet(d, p, t);
                    solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
                }
            }
        }
}

void Prover::noN5TimeNeighbor()
{
    int xNum, yNum;
    pf.getSize(xNum, yNum);
    // N-5 neighbor
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1 不存在
            {
                expr exprNeighbor = cxt.int_val(0);
                for (int d2 = 0; d2 < pf.getDropletNum(); ++d2) // 无对称性，必须都算
                {
                    if (d2 == d)
                        continue;
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeDroplet(d2, p2, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                    { // 不去心
                        int sequenceNum2 = formu.computeDroplet(d2, p, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                {
                    int sequenceNum = formu.computeDroplet(d, p, t);
                    solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
                }
            }
        }
}

void Prover::enoughNumber()
{
    // droplet，同时确保所有mix进行
    for (int d = 0; d < pf.getDropletNum(); ++d)
    {
        expr exprEnough = cxt.int_val(0);
        for (int p = 0; p < pf.getSize(); ++p)
            for (int t = 0; t < pf.getTime(); ++t)
            {
                int sequenceNum = formu.computeDroplet(d, p, t);
                exprEnough = exprEnough + exprVec[sequenceNum];
            }
        solv.add(exprEnough >= 1);
    }

    // detector
    for (int n = 0; n < pf.getDetectorNum(); ++n)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int sequenceNum = formu.computeDetector(n, p);
            solv.add(exprVec[sequenceNum] == 1);
        }

    // dispenser
    for (int n = 0; n < pf.getDispenserNum(); ++n)
        for (int e = 0; e < pf.getEdgeNum(); ++e)
        {
            int sequenceNum = formu.computeDispenser(n, e);
            solv.add(exprVec[sequenceNum] == 1);
        }

    // sinker
    {
        expr exprEnough = cxt.int_val(0);
        for (int e = 0; e < pf.getEdgeNum(); ++e)
        {
            int sequenceNum = formu.computeSinker(e);
            exprEnough = exprEnough + exprVec[sequenceNum];
        }
        solv.add(exprEnough == pf.getSinkerNum());
    }
}

void Prover::operationMovement()
{
    int xNum, yNum;
    pf.getSize(xNum, yNum);
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            expr exprDispenser = cxt.int_val(0);
            {
                // dispenser produce
                vector<int> e;
                pf.computeAroundChip(p, e);
                for (int i = 0; i < e.size(); ++i)
                {
                    int sequenceNum = formu.computeDispenser(pf.findDetectorOfDroplet(d), e[i]);
                    exprDispenser = exprDispenser + exprVec[sequenceNum];
                }
            }
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1不存在
            {
                expr exprMovement = cxt.int_val(0);
                // move from N-5 neighbor
                for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                {
                    int x2 = x + N5X[i];
                    int y2 = y + N5Y[i];
                    if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                        continue;
                    int p2 = pf.computePosition(x2, y2);
                    int sequenceNum2 = formu.computeDroplet(d, p2, t - 1);
                    exprMovement = exprMovement + exprVec[sequenceNum2];
                }
                { // 不去心
                    int sequenceNum2 = formu.computeDroplet(d, p, t - 1);
                    exprMovement = exprMovement + exprVec[sequenceNum2];
                }
                // mixer produce
                int mixerNum = pf.findMixerAsDroplet(d);
                if (mixerNum >= 0)
                {
                    int sequenceNum = formu.computeMixer(mixerNum, p, t);
                    exprMovement = exprMovement + exprVec[sequenceNum];
                }
                {
                    int sequenceNum = formu.computeDroplet(d, p, t);
                    solv.add(implies(exprVec[sequenceNum] == 1, exprMovement == 1) || implies(exprVec[sequenceNum] == 1, exprDispenser >= 1));
                }
            }
        }
}

void Prover::operationDisappear()
{
    int xNum, yNum;
    pf.getSize(xNum, yNum);
    for (int p = 0; p < pf.getSize(); ++p)
    {
        expr exprSinker = cxt.int_val(0);
        {
            // sinker produce
            vector<int> e;
            pf.computeAroundChip(p, e);
            for (int i = 0; i < e.size(); ++i)
            {
                int sequenceNum = formu.computeSinker(e[i]);
                exprSinker = exprSinker + exprVec[sequenceNum];
            }
        }
        int x, y;
        pf.computeXY(x, y, p);
        for (int d = 0; d < pf.getDropletNum(); ++d)
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1不存在
            {
                // produce mixer
                expr exprMixer = cxt.int_val(0);
                expr exprMixerPre = cxt.int_val(0);
                int mixerNum = pf.findMixerAsDroplet(d);
                if (mixerNum >= 0)
                {
                    int sequenceNum = formu.computeMixer(mixerNum, p, t);
                    exprMixer = exprMixer + exprVec[sequenceNum];
                    int sequenceNumPre = formu.computeMixer(mixerNum, p, t);
                    exprMixerPre = exprMixerPre + exprVec[sequenceNumPre];
                }
                // neighbor
                expr exprNeighbor = cxt.int_val(0);
                for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                {
                    int x2 = x + N5X[i];
                    int y2 = y + N5Y[i];
                    if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                        continue;
                    int p2 = pf.computePosition(x2, y2);
                    int sequenceNum2 = formu.computeDroplet(d, p2, t);
                    exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                }
                { // 不去心
                    int sequenceNum2 = formu.computeDroplet(d, p, t);
                    exprNeighbor = exprNeighbor + exprVec[sequenceNum];
                }
                {
                    int sequenceNum = formu.computeDroplet(d, p, t - 1);
                    solv.add(implies(exprVec[sequenceNum] == 1 && exprNeighbor == 0, exprSinker >= 1) ||
                             implies(exprVec[sequenceNum] == 1 && exprNeighbor == 0, exprMixer >= 1 && exprMixerPre == 0));
                }
            }
    }
}

void Prover::operationMixing() {}
void Prover::operationDetecting()
{
    /*for (int n = 0; n < pf.getDetectorNum(); ++n)
        for (int t = 0; t < pf.getTime(); ++t)
            for (int x = 0; x < xNum; ++x)
                for (int y = 0; y < yNum; ++y)
                {
                    int dimension1, dimension2;
                    formu.computeDetector(dimension1, dimension2, n, x, y);
                    solv.add(exprVec)
                }*/
}

void Prover::meetObjective() {}

} // namespace DMFB