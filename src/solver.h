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
    Solver(const Profile &p) : pf(p), objective(""){};
    ~Solver(){};

    void solve();
    void print();

    const std::string &getObjective() const
    {
        return this->objective;
    }
    void setObjective(const std::string &o)
    {
        assert(o.compare("prove") == 0 || o.compare("min time") == 0 || o.compare("min size") == 0);
        objective = o;
    }

  private:
    Profile pf;
    std::string objective;

    void _solve();

    const int MAXTIME = 20;
    const int MAXLENGTH = 8;
};
}; // namespace DMFB

#endif // SOLVER_H_