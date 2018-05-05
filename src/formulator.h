#ifndef FORMULATOR_H_
#define FORMULATOR_H_

#include <vector>
#include "profile.h"
#include "z3++.h"

namespace DMFB
{

/**
 * @brief Formulate configuration and profile into z3Solver context.
 * 
 */
class Formulator
{
  public:
    Formulator(const Profile &p, z3::context &c) : pf(p), cxt(c);
    ~Formulator(){};

    inline z3::context &getContext() const;
    inline const std::vector<z3::expr_vector> &getExprVector() const;
    const void computeDroplet(int &dimension1, int &dimension2, int droplet, int x, int y, int time) const;
    const void computeDroplet(int &dimension1, std::vector<int> &dimension2, int net, int x, int y, int time) const;
    const void computeDetector(int &dimension1, int &dimension2, int n, int x, int y) const;
    const void computeDispenser(int &dimension1, int &dimension2, int n, int edge) const;
    const void computeSinker(int &dimension1, int &dimension2, int edge) const;
    const bool computeAroundChip(int x, int y, std::vector<int> edge) const;
    void formulate();

  private:
    const Profile &pf;
 

    int xNum, yNum;

    void formulateDroplet();
    void formulateDetector();
    void formulateDispenser();
    void formulateSinker();
    void formulateDetecting();

    Formulator(Formulator &);
    Formulator &operator=(Formulator &);
};

/*******************functions********************/
inline z3::context &Formulator::getContext() const
{
    return const_cast<z3::context &>(this->cxt);
}

inline const std::vector<z3::expr_vector> &Formulator::getExprVector() const
{
    return this->exprVec;
}

} // namespace DMFB

#endif // FORMULATOR_H_