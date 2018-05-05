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
    Prover(const Profile &p, const Formulator &f);
    ~Prover(){};

    void prove();

  private:
    const Profile &pf;
    const Formulator &formu;

    z3::context &cxt;
    const std::vector<z3::expr_vector> &exprVec;
    int xNum, yNum;

    void isBool();
    void noSpaceOverlap();
    void noTimeOverlap();
    void noSpaceNeighbor();
    void noTimeNeighbor();
    void enoughNumber();
    void operationMovement();
    void operationDisappear();
    void operationMixing();
    void operationDetecting();
    void meetObjective();

    static const int N9X[8];
    static const int N9Y[8];
    static const int N5X[4];
    static const int N5Y[4];

    Prover(Prover &);
    Prover &operator=(Prover &);
};

} // namespace DMFB
#endif // PROVER_H_