#ifndef SOLVER_H_
#define SOLVER_H_

#include <string>
#include <cassert>
#include "profile.h"
#include "z3++.h"

namespace DMFB
{
/**
 * @brief DMFB solver
 * 
 */
class Solver
{
public:
  /*********************constructor and destructor******************/
  Solver(Profile &p, const std::string &o = "");
  ~Solver(){};

  void solve();

  /********************************getter*************************/
  const std::string &getObjective() const
  {
    return this->objective;
  }

  /*******************************setter**********************/
  void setObjective(const std::string &o)
  {
    assert(o.compare("prove") == 0 || o.compare("min time") == 0 || o.compare("min size") == 0);
    objective = o;
  }

private:
  Profile &pf;
  std::string objective;
  z3::context cxt;
  z3::expr_vector exprVec;
  z3::solver solv;

  void _solve();
  void checkReadyForSolve();

  const int MAXTIME = 20;
  const int MAXLENGTH = 8;
};

} // namespace DMFB

#endif // SOLVER_H_