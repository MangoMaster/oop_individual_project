#include <cstdlib>
#include <string>
#include <iostream>
#include <cassert>
#include "solver.h"
#include "z3++.h"

using namespace std;

namespace DMFB
{

void Solver::solve()
{
    assert(objective.compare("") != 0);
    assert(!pf.getDropletVec().empty());
    if (objective.compare("prove") == 0)
    {
        int pfX, pfY;
        pf.getSize(pfX, pfY);
        assert(pfX != 0 && pfY != 0 && pf.getTime() != 0);
        _solve();
    }
    else if (objective.compare("min time") == 0)
    {
        int pfX, pfY;
        pf.getSize(pfX, pfY);
        assert(pfX != 0 && pfY != 0);
        //TODO:recursive, modify pf
        for (int tempTime = 1; tempTime < MAXTIME; ++tempTime)
        {
            pf.setTime(tempTime);
            _solve();
        }
    }
    else if (objective.compare("min size") == 0)
    {
        assert(pf.getTime() != 0);
        //TODO:recursive, modify pf
        for (int tempX = 1; tempX < MAXLENGTH; ++tempX)
            for (int tempY = tempX; tempY < MAXLENGTH; ++tempY)
            {
                pf.setSize(tempX, tempY);
                _solve();
            }
    }
}

void Solver::_solve()
{
    Formulator f(pf);
    f.formulate();
    Prover p(f);
    p.prove();
}

void Solver::print()
{
}

} // namespace DMFB