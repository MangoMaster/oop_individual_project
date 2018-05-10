#include <iostream>
#include <fstream>
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
    solv.reset(); // 每次prove的时候都需重置solv
    isBool();
    isStart();
    noSpaceOverlap();
    noTimeOverlap();
    noN9SpaceNeighbor();
    noN9TimeNeighbor();
    enoughNumber();
    operationMovement();
    operationDisappear();
    operationMixing();
    operationDetecting();
}

void Prover::isBool()
{
    // 0-false  1-true
    for (int i = 0; i < exprVec.size(); ++i)
        solv.add(exprVec[i] == 0 || exprVec[i] == 1);
}

void Prover::isStart()
{
    // droplet t==0
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int sequenceNum = formu.computeDroplet(d, p, 0);
            solv.add(exprVec[sequenceNum] == 0);
        }

    // droplet produced by mixer t==[0, mixTime+2)
    for (int d = 0; d < pf.getDropletNum(); ++d)
    {
        int mixerNum = pf.findMixerAsDroplet(d);
        if (mixerNum >= 0)
            for (int p = 0; p < pf.getSize(); ++p)
                for (int t = 0; t < pf.getMixerVec()[mixerNum].getMixTime() + 2 && t < pf.getTime(); ++t)
                {
                    int sequenceNum = formu.computeDroplet(d, p, t);
                    solv.add(exprVec[sequenceNum] == 0);
                }
    }

    // mixer t==0
    for (int n = 0; n < pf.getMixerNum(); ++n)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int sequenceNum = formu.computeMixer(n, p, 0);
            solv.add(exprVec[sequenceNum] == 0);
        }
}

void Prover::noSpaceOverlap()
{
    // droplet & mixer
    for (int p = 0; p < pf.getSize(); ++p)
        for (int t = 0; t < pf.getTime(); ++t)
        {
            expr exprDroplet = cxt.int_val(0);
            for (int d = 0; d < pf.getDropletNum(); ++d)
            {
                int sequenceNum = formu.computeDroplet(d, p, t);
                exprDroplet = exprDroplet + exprVec[sequenceNum];
            }
            for (int n = 0; n < pf.getMixerNum(); ++n)
            {
                int sequenceNum = formu.computeMixer(n, p, t);
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

    // mixer
    for (int n = 0; n < pf.getMixerNum(); ++n)
        for (int t = 0; t < pf.getTime(); ++t)
        {
            expr exprMixer = cxt.int_val(0);
            for (int p = 0; p < pf.getSize(); ++p)
            {
                int sequenceNum = formu.computeMixer(n, p, t);
                exprMixer = exprMixer + exprVec[sequenceNum];
            }
            int mixerSize = pf.getMixerVec()[n].getMixSize();
            solv.add(exprMixer == mixerSize || exprMixer == 0);
        }
}

void Prover::noN9SpaceNeighbor()
{
    // N-9 neighbor
    int xNum, yNum;
    pf.getSize(xNum, yNum);

    // droplet
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 0; t < pf.getTime(); ++t)
            {
                expr exprNeighbor = cxt.int_val(0);
                for (int d2 = d + 1; d2 < pf.getDropletNum(); ++d2) // 由对称性只需计算一半
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
                int sequenceNum = formu.computeDroplet(d, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }

    // mixer
    for (int n = 0; n < pf.getMixerNum(); ++n)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 0; t < pf.getTime(); ++t)
            {
                expr exprNeighbor = cxt.int_val(0);
                // mixer neighbor
                for (int n2 = n + 1; n2 < pf.getMixerNum(); ++n2) // 由对称性只需计算一半
                    for (int i = 0; i < sizeof(N9X) / sizeof(N9X[0]); ++i)
                    {
                        int x2 = x + N9X[i];
                        int y2 = y + N9Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeMixer(n2, p2, t);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                // droplet neighbor
                for (int d2 = 0; d2 < pf.getDropletNum(); ++d2) // 由对称性只需算mixer到droplet
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
                int sequenceNum = formu.computeMixer(n, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }
}

void Prover::noN5SpaceNeighbor()
{
    // N-5 neighbor
    int xNum, yNum;
    pf.getSize(xNum, yNum);

    // droplet
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 0; t < pf.getTime(); ++t)
            {
                expr exprNeighbor = cxt.int_val(0);
                for (int d2 = d + 1; d2 < pf.getDropletNum(); ++d2) // 由对称性只需计算一半
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
                int sequenceNum = formu.computeDroplet(d, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }

    // mixer
    for (int n = 0; n < pf.getMixerNum(); ++n)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 0; t < pf.getTime(); ++t)
            {
                expr exprNeighbor = cxt.int_val(0);
                // mixer neighbor
                for (int n2 = n + 1; n2 < pf.getMixerNum(); ++n2) // 由对称性只需计算一半
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeMixer(n2, p2, t);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                // droplet neighbor
                for (int d2 = 0; d2 < pf.getDropletNum(); ++d2) // 由对称性只需算mixer到droplet
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
                int sequenceNum = formu.computeMixer(n, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }
}

void Prover::noN9TimeNeighbor()
{
    // N-9 neighbor
    int xNum, yNum;
    pf.getSize(xNum, yNum);

    // droplet
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1 不存在
            {
                expr exprNeighbor = cxt.int_val(0);
                // droplet neighbor
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
                // mixer neighbor
                int mixerNum1 = pf.findMixerOfDroplet(d);
                int mixerNum2 = pf.findMixerAsDroplet(d);
                for (int n2 = 0; n2 < pf.getMixerNum(); ++n2) // 无对称性，必须都算
                {
                    if (n2 == mixerNum1 || n2 == mixerNum2)
                        continue;
                    for (int i = 0; i < sizeof(N9X) / sizeof(N9X[0]); ++i)
                    {
                        int x2 = x + N9X[i];
                        int y2 = y + N9Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeMixer(n2, p2, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                    { // 不去心
                        int sequenceNum2 = formu.computeMixer(n2, p, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                int sequenceNum = formu.computeDroplet(d, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }

    // mixer
    for (int n = 0; n < pf.getMixerNum(); ++n)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1 不存在
            {
                expr exprNeighbor = cxt.int_val(0);
                // mixer neighbor
                for (int n2 = 0; n2 < pf.getMixerNum(); ++n2) // 无对称性，必须都算
                {
                    if (n2 == n)
                        continue;
                    for (int i = 0; i < sizeof(N9X) / sizeof(N9X[0]); ++i)
                    {
                        int x2 = x + N9X[i];
                        int y2 = y + N9Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeMixer(n2, p2, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                    { // 不去心
                        int sequenceNum2 = formu.computeMixer(n2, p, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                // droplet neighbor
                int dropletNum1, dropletNum2;
                pf.findDropletOfMixer(n, dropletNum1, dropletNum2);
                for (int d2 = 0; d2 < pf.getDropletNum(); ++d2) // 无对称性，必须都算
                {
                    if (d2 == dropletNum1 || d2 == dropletNum2)
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
                int sequenceNum = formu.computeMixer(n, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }
}

void Prover::noN5TimeNeighbor()
{
    // N-5 neighbor
    int xNum, yNum;
    pf.getSize(xNum, yNum);

    // droplet
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1 不存在
            {
                expr exprNeighbor = cxt.int_val(0);
                // droplet neighbor
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
                // mixer neighbor
                int mixerNum1 = pf.findMixerOfDroplet(d);
                int mixerNum2 = pf.findMixerAsDroplet(d);
                for (int n2 = 0; n2 < pf.getMixerNum(); ++n2) // 无对称性，必须都算
                {
                    if (n2 == mixerNum1 || n2 == mixerNum2)
                        continue;
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeMixer(n2, p2, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                    { // 不去心
                        int sequenceNum2 = formu.computeMixer(n2, p, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                int sequenceNum = formu.computeDroplet(d, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
            }
        }

    // mixer
    for (int n = 0; n < pf.getMixerNum(); ++n)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1 不存在
            {
                expr exprNeighbor = cxt.int_val(0);
                // mixer neighbor
                for (int n2 = 0; n2 < pf.getMixerNum(); ++n2) // 无对称性，必须都算
                {
                    if (n2 == n)
                        continue;
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeMixer(n2, p2, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                    { // 不去心
                        int sequenceNum2 = formu.computeMixer(n2, p, t - 1);
                        exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                    }
                }
                // droplet neighbor
                int dropletNum1, dropletNum2;
                pf.findDropletOfMixer(n, dropletNum1, dropletNum2);
                for (int d2 = 0; d2 < pf.getDropletNum(); ++d2) // 无对称性，必须都算
                {
                    if (d2 == dropletNum1 || d2 == dropletNum2)
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
                int sequenceNum = formu.computeMixer(n, p, t);
                solv.add(implies(exprVec[sequenceNum] == 1, exprNeighbor == 0));
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

    // mixer
    for (int n = 0; n < pf.getMixerNum(); ++n)
    {
        expr exprEnough = cxt.int_val(0);
        for (int p = 0; p < pf.getSize(); ++p)
            for (int t = 0; t < pf.getTime(); ++t)
            {
                int sequenceNum = formu.computeMixer(n, p, t);
                exprEnough = exprEnough + exprVec[sequenceNum];
            }
        solv.add(exprEnough == pf.getMixerVec()[n].getMixSize() * pf.getMixerVec()[n].getMixTime());
    }

    // detector
    for (int n = 0; n < pf.getDetectorNum(); ++n)
    {
        expr exprEnough = cxt.int_val(0);
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int sequenceNum = formu.computeDetector(n, p);
            exprEnough = exprEnough + exprVec[sequenceNum];
        }
        solv.add(exprEnough == 1);
    }

    // dispenser
    for (int n = 0; n < pf.getDispenserNum(); ++n)
    {
        expr exprEnough = cxt.int_val(0);
        for (int e = 0; e < pf.getEdgeNum(); ++e)
        {
            int sequenceNum = formu.computeDispenser(n, e);
            exprEnough = exprEnough + exprVec[sequenceNum];
        }
        solv.add(exprEnough == 1);
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

    // detecting
    for (int n = 0; n < pf.getDropletNum(); ++n)
    {
        int detectorNum = pf.findDetectorOfDroplet(n);
        if (detectorNum >= 0) // 有detect
        {
            int detectTime = pf.getDetectorVec()[detectorNum].getDetectTime();
            expr exprDetecting = cxt.int_val(0);
            for (int t = 0; t < pf.getTime(); ++t)
            {
                int sequenceNum = formu.computeDetecting(n, t);
                exprDetecting = exprDetecting + exprVec[sequenceNum];
            }
            solv.add(exprDetecting == detectTime);
        }
        else // 无detect
            for (int t = 0; t < pf.getTime(); ++t)
            {
                int sequenceNum = formu.computeDetecting(n, t);
                solv.add(exprVec[sequenceNum] == 0);
            }
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
                int dispenserNum = pf.findDispenserOfDroplet(d);
                if (dispenserNum >= 0) // 有dispenser
                {
                    vector<int> e;
                    pf.computeAroundChip(p, e);
                    for (int i = 0; i < e.size(); ++i)
                    {
                        int sequenceNum = formu.computeDispenser(dispenserNum, e[i]);
                        exprDispenser = exprDispenser + exprVec[sequenceNum];
                    }
                }
            }
            int x, y;
            pf.computeXY(x, y, p);
            for (int t = 1; t < pf.getTime(); ++t) // t==0 时 t-1不存在
            {
                // move from N-5 neighbor
                expr exprMovement = cxt.int_val(0);
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
                expr exprMixer = cxt.int_val(0);
                int mixerNum = pf.findMixerAsDroplet(d);
                if (mixerNum >= 0) // 有mixer
                {
                    int sequenceNum = formu.computeMixer(mixerNum, p, t - 1);
                    exprMixer = exprMixer + exprVec[sequenceNum];
                }
                {
                    int sequenceNum = formu.computeDroplet(d, p, t);
                    solv.add(implies(exprVec[sequenceNum] == 1, exprMovement == 1) ||
                             implies(exprVec[sequenceNum] == 1, exprDispenser >= 1) ||
                             implies(exprVec[sequenceNum] == 1, exprMixer == 1));
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
                int mixerNum = pf.findMixerOfDroplet(d);
                if (mixerNum >= 0)
                {
                    for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                    {
                        int x2 = x + N5X[i];
                        int y2 = y + N5Y[i];
                        if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                            continue;
                        int p2 = pf.computePosition(x2, y2);
                        int sequenceNum2 = formu.computeMixer(mixerNum, p2, t);
                        exprMixer = exprMixer + exprVec[sequenceNum2];
                        int sequenceNumPre2 = formu.computeMixer(mixerNum, p2, t - 1);
                        exprMixerPre = exprMixerPre + exprVec[sequenceNumPre2];
                    }
                    { // 不去心
                        int sequenceNum = formu.computeMixer(mixerNum, p, t);
                        exprMixer = exprMixer + exprVec[sequenceNum];
                        int sequenceNumPre = formu.computeMixer(mixerNum, p, t - 1);
                        exprMixerPre = exprMixerPre + exprVec[sequenceNumPre];
                    }
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
                    exprNeighbor = exprNeighbor + exprVec[sequenceNum2];
                }
                {
                    int sequenceNum = formu.computeDroplet(d, p, t - 1);
                    solv.add(implies(exprVec[sequenceNum] == 1 && exprNeighbor == 0, exprSinker >= 1) ||
                             implies(exprVec[sequenceNum] == 1 && exprNeighbor == 0, exprMixer >= 1 && exprMixerPre == 0));
                }
            }
    }
}

void Prover::operationMixing()
{
    for (int n = 0; n < pf.getDropletNum(); ++n)
    {
        int mixerNum = pf.findMixerAsDroplet(n);
        if (mixerNum >= 0)
        {
            assert(pf.findDispenserOfDroplet(n) < 0); // no dispenser
            int mixTime = pf.getMixerVec()[mixerNum].getMixTime();
            int mixSize = pf.getMixerVec()[mixerNum].getMixSize();
            for (int p = 0; p < pf.getSize(); ++p)
                for (int t = mixTime + 2; t < pf.getTime(); ++t)
                {
                    // t时刻冒出来
                    int sequenceNum = formu.computeDroplet(n, p, t);
                    // t-1时刻哪也没有
                    expr exprDropletPre = cxt.int_val(0);
                    for (int p2 = 0; p2 < pf.getSize(); ++p2)
                    {
                        int sequenceNumPre = formu.computeDroplet(n, p2, t - 1);
                        exprDropletPre = exprDropletPre + exprVec[sequenceNumPre];
                    }
                    int oldDropletNum1, oldDropletNum2;
                    pf.findDropletOfMixer(mixerNum, oldDropletNum1, oldDropletNum2);
                    // t-mixTime-1时刻oldDroplet在N-5邻域
                    expr exprOldDropletPre1 = cxt.int_val(0);
                    expr exprOldDropletPre2 = cxt.int_val(0);
                    {
                        int x, y;
                        pf.computeXY(x, y, p);
                        int xNum, yNum;
                        pf.getSize(xNum, yNum);
                        for (int i = 0; i < sizeof(N5X) / sizeof(N5X[0]); ++i)
                        {
                            int x2 = x + N5X[i];
                            int y2 = y + N5Y[i];
                            if (x2 < 0 || x2 >= xNum || y2 < 0 || y2 >= yNum)
                                continue;
                            int p2 = pf.computePosition(x2, y2);
                            int sequenceNumOldPre1 = formu.computeDroplet(oldDropletNum1, p2, t - mixTime - 1);
                            exprOldDropletPre1 = exprOldDropletPre1 + exprVec[sequenceNumOldPre1];
                            int sequenceNumOldPre2 = formu.computeDroplet(oldDropletNum2, p2, t - mixTime - 1);
                            exprOldDropletPre2 = exprOldDropletPre2 + exprVec[sequenceNumOldPre2];
                        }
                    }
                    solv.add(implies(exprVec[sequenceNum] == 1 && exprDropletPre == 0,
                                     exprOldDropletPre1 == 1 && exprOldDropletPre2 == 1));

                    // t-mixTime时刻oldDroplet不见了
                    expr exprOldDroplet1 = cxt.int_val(0);
                    expr exprOldDroplet2 = cxt.int_val(0);
                    for (int p2 = 0; p2 < pf.getSize(); ++p2)
                    {
                        int sequenceNumOld1 = formu.computeDroplet(oldDropletNum1, p2, t - mixTime);
                        exprOldDroplet1 = exprOldDroplet1 + exprVec[sequenceNumOld1];
                        int sequenceNumOld2 = formu.computeDroplet(oldDropletNum2, p2, t - mixTime);
                        exprOldDroplet2 = exprOldDroplet2 + exprVec[sequenceNumOld2];
                    }
                    solv.add(implies(exprVec[sequenceNum] == 1 && exprDropletPre == 0,
                                     exprOldDroplet1 == 0 && exprOldDroplet2 == 0));

                    // [t-mixTime,t)进行mix
                    expr exprMixGraph = cxt.bool_val(false);
                    {
                        vector<vector<int> > mixerGraph;
                        pf.computeGraph(mixerNum, p, mixerGraph);
                        for (int tempGraphNum = 0; tempGraphNum < mixerGraph.size(); ++tempGraphNum)
                        {
                            expr exprGraphNum = cxt.int_val(0);
                            assert(mixerGraph[tempGraphNum].size() == mixSize);
                            for (int tempPositionNum = 0; tempPositionNum < mixerGraph[tempGraphNum].size(); ++tempPositionNum)
                                for (int t2 = t - mixTime; t2 < t; ++t2)
                                {
                                    int sequenceNumMixer = formu.computeMixer(mixerNum, mixerGraph[tempGraphNum][tempPositionNum], t2);
                                    exprGraphNum = exprGraphNum + exprVec[sequenceNumMixer];
                                }
                            exprMixGraph = exprMixGraph || (exprGraphNum == mixSize * mixTime);
                        }
                    }
                    solv.add(implies(exprVec[sequenceNum] == 1 && exprDropletPre == 0,
                                     exprMixGraph));
                }
        }
    }
}

void Prover::operationDetecting()
{
    for (int n = 0; n < pf.getDropletNum(); ++n)
    {
        int detectorNum = pf.findDetectorOfDroplet(n);
        if (detectorNum >= 0) // 有detect
        {
            int detectTime = pf.getDetectorVec()[detectorNum].getDetectTime();
            for (int p = 0; p < pf.getSize(); ++p)
                for (int t = 1; t < pf.getTime() - detectTime; ++t) // 满足要求的t
                {
                    int detectingSequenceNumt = formu.computeDetecting(n, t);
                    int detectingSequenceNumtPre = formu.computeDetecting(n, t - 1);
                    int dropletSequenceNumtPre = formu.computeDroplet(n, p, t - 1);
                    for (int t2 = t + 1; t2 < t + detectTime; ++t2)
                    {
                        int detectingSequenceNumt2 = formu.computeDetecting(n, t2);
                        int dropletSequenceNumt2 = formu.computeDroplet(n, p, t2);
                        solv.add(implies(exprVec[detectingSequenceNumt] == 1 &&
                                             exprVec[detectingSequenceNumtPre] == 0,
                                         exprVec[dropletSequenceNumtPre] == 0 &&
                                             exprVec[detectingSequenceNumt2] == 1 &&
                                             exprVec[dropletSequenceNumt2] == 1));
                    }
                }
        }
    }

    for (int n = 0; n < pf.getDropletNum(); ++n)
    {
        int detectorNum = pf.findDetectorOfDroplet(n);
        if (detectorNum >= 0) // 有detect
        {
            for (int t = 0; t < pf.getTime(); ++t)
            {
                int detectingSequenceNum = formu.computeDetecting(n, t);
                expr exprDroplet = cxt.int_val(0);
                for (int p = 0; p < pf.getSize(); ++p)
                {
                    int dropletSequenceNum = formu.computeDroplet(n, p, t);
                    exprDroplet = exprDroplet + exprVec[dropletSequenceNum];
                }
                solv.add(implies(exprVec[detectingSequenceNum] == 1, exprDroplet == 1));
            }
        }
    }

    for (int n = 0; n < pf.getDropletNum(); ++n)
    {
        int detectorNum = pf.findDetectorOfDroplet(n);
        if (detectorNum >= 0) // 有detect
        {
            int detectTime = pf.getDetectorVec()[detectorNum].getDetectTime();
            for (int p = 0; p < pf.getSize(); ++p)
            {
                int detectorSequenceNum = formu.computeDetector(detectorNum, p);
                for (int t = 0; t < pf.getTime(); ++t)
                {
                    int detectingSequenceNum = formu.computeDetecting(n, t);
                    int dropletSequenceNum = formu.computeDroplet(n, p, t);
                    solv.add(implies(exprVec[detectingSequenceNum] == 1 && exprVec[dropletSequenceNum] == 1, exprVec[detectorSequenceNum] == 1));
                }
            }
        }
    }
}

} // namespace DMFB