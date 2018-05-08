#include <string>
#include <cassert>
#include "solver.h"
#include "formulator.h"
#include "prover.h"
#include "printer.h"
#include "z3++.h"

using namespace std;
using namespace z3;

namespace DMFB
{

Solver::Solver(Profile &p, const std::string &o /* = ""*/)
    : pf(p), objective(o), cxt(), exprVec(cxt), solv(cxt)
{
    assert(o.compare("") == 0 || o.compare("prove") == 0 || o.compare("min time") == 0 || o.compare("min size") == 0);
}

void Solver::solve()
{
    checkReadyForSolve();
    if (objective.compare("prove") == 0)
    {
        int pfX, pfY;
        pf.getSize(pfX, pfY);
        assert(pfX >= 0 && pfY >= 0 && pf.getTime() >= 0);
        _solve();
    }
    else if (objective.compare("min time") == 0)
    {
        int pfX, pfY;
        pf.getSize(pfX, pfY);
        assert(pfX >= 0 && pfY >= 0);
        //loop and increase time
        for (int tempTime = 1; tempTime <= MAXTIME; ++tempTime)
        {
            pf.setTime(tempTime);
            _solve();
            cout << tempTime;
            if (solv.check() == sat)
                break;
        }
    }
    else if (objective.compare("min size") == 0)
    {
        assert(pf.getTime() != 0);
        //loop and increase size
        for (int tempY = 1; tempY <= MAXLENGTH; ++tempY)
        {
            for (int tempX = tempY; tempX <= MAXLENGTH; ++tempX) // x>=y
            {
                pf.setSize(tempX, tempY);
                _solve();
                cout << "(" << tempX << tempY << ")";
                if (solv.check() == sat)
                    break;
            }
            if (solv.check() == sat)
                break;
        }
    }
    else
        assert(false);
}

void Solver::print()
{
    Printer p(solv);
    p.print();
}

void Solver::_solve()
{
    Formulator f(pf, cxt, exprVec);
    f.formulate();
    Prover p(pf, exprVec, f, cxt, solv);
    p.prove();
}

void Solver::checkReadyForSolve()
{
    assert(objective.compare("") != 0);
    assert(pf.getDropletNum() != 0);
    // all droplets are either produced by a dispenser or produced by a mixer
    for (int i = 0; i < pf.getDropletNum(); ++i)
        assert(pf.findDispenserOfDroplet(i) >= 0 || pf.findMixerAsDroplet(i) >= 0);
}

} // namespace DMFB