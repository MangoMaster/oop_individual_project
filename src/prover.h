#ifndef PORVER_H_
#define PORVER_H_

#include "profile.h"
#include "formulator.h"

namespace DMFB
{
/**
 * @brief prove the satisfiability using z3Solver solver.
 * 
 */
class Prover
{
public:
  /******************constructor and destructor********************/
  Prover(const Profile &p, const z3::expr_vector &ev, const Formulator &f, z3::context &c, z3::solver &s);
  ~Prover(){};

  /************************main function***************************/
  void prove();

private:
  const Profile &pf;
  const z3::expr_vector &exprVec;
  const Formulator &formu;
  z3::context &cxt;
  z3::solver &solv;

  /***********************assistant function**********************/
  void isBool();
  void noSpaceOverlap();
  void noTimeOverlap();
  void noN9SpaceNeighbor();
  void noN5SpaceNeighbor();
  void noN9TimeNeighbor();
  void noN5TimeNeighbor();
  void enoughNumber();
  void operationMovement();
  void operationDisappear();
  void operationMixing();
  void operationDetecting();

  static const int N9X[8];
  static const int N9Y[8];
  static const int N5X[4];
  static const int N5Y[4];

  Prover(Prover &);
  Prover &operator=(Prover &);
};

} // namespace DMFB
#endif // PROVER_H_