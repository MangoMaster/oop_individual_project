#include <iostream>
#include <vector>
#include <cassert>
#include "solver.h"

using namespace std;
using namespace z3;

namespace DMFB
{
Solver::Prover::Prover(Formulator &f)
    : formu(f), cxt(f.getContext()), solv(f.getContext()), exprVec(f.getExprVector())
{
    netNum = formu.getNetNum();
    formu.getSizeNum(xNum, yNum);
    tNum = formu.getTNum();
}

void Solver::Prover::prove()
{
    //TODO:
}

void Solver::Prover::noOverlap()
{
    // droplet
    for (int x = 0; x < xNum; ++x)
        for (int y = 0; y < yNum; ++y)
            for (int t = 0; t < tNum; ++t)
            {
                expr expr1 = cxt.int_val(0);
                for (int n = 0; n < netNum; ++n)
                {
                    int dimension1;
                    vector<int> dimension2;
                    formu.computeDroplet(dimension1, dimension2, n, x, y, t);
                    for (int temp = 0; temp < dimension2.size(); ++temp)
                        expr1 = expr1 + exprVec[dimension1][temp];
                }
                solv.add(expr1);
            }

    // detector
    for (int x = 0; x < xNum; ++x)
        for (int y = 0; y < yNum; ++y)
            {
                expr detectorNoSpaceOverlap = cxt.bool_val(true);
                for (int n = 0; n < netNum; ++n)
                {
                    int dimension1, dimension2;
                    formu.computeDetector(dimension1, dimension2, n, x, y);
                    detectorNoSpaceOverlap = detectorNoSpaceOverlap && exprVec[dimension1][dimension2];
                }
            }

    // dispenser & sinker
}

void Solver::Prover::noSpaceNeighbor() {}
void Solver::Prover::noTimeNeighbor() {}
void Solver::Prover::enoughSize() {}
void Solver::Prover::operationMovement() {}
void Solver::Prover::operationMixing() {}
void Solver::Prover::operationDetecting() {}
void Solver::Prover::meetObjective() {}

} // namespace DMFB