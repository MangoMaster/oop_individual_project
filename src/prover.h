#ifndef PROVER_H_
#define PROVER_H_

#include "z3++.h"

namespace DMFB
{
/**
 * @brief prove the satisfiability using z3Solver solver.
 * 
 */
class Prover
{
public:
  Prover(z3::context &c) : solv(c){};
  ~Prover(){};

  void prove();

private:
  z3::solver solv;
};
};

#endif // PROVER_H_