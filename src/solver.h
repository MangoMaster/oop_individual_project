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

    class Formulator;
    class Prover;

    const int MAXTIME = 20;
    const int MAXLENGTH = 8;
};

/**
 * @brief Formulate configuration and profile into z3Solver context.
 * 
 */
class Solver::Formulator
{
  public:
    Formulator(const Profile &p) : pf(p), cxt(){};
    ~Formulator(){};

    inline z3::context &getContext() const;
    void formulate();

  private:
    const Profile &pf;
    z3::context cxt;

    Formulator(Formulator &);
    Formulator &operator=(Formulator &);
};

/**
 * @brief prove the satisfiability using z3Solver solver.
 * 
 */
class Solver::Prover
{
  public:
    Prover(z3::context &c) : solv(c){};
    ~Prover(){};

    void prove();

  private:
    z3::solver solv;
};

/*******************functions********************/
inline z3::context &Solver::Formulator::getContext() const
{
    return const_cast<z3::context &>(this->cxt);
}

}; // namespace DMFB

#endif // SOLVER_H_