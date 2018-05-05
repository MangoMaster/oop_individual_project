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
  /*********************constructor and destructor******************/
  Formulator(const Profile &p, z3::context &c, z3::expr_vector &ev);
  ~Formulator(){};

  /************************computer*****************************/
  int computeDroplet(int dropletSequenceNum, int position, int time) const;
  int computeMixer(int mixerSequenceNum, int position, int time) const;
  int computeDetector(int detectorSequenceNum, int position) const;
  int computeDispenser(int dispenserSequenceNum, int edge) const;
  int computeSinker(int edge) const;

  /*********************main function*************************/
  void formulate();

private:
  const Profile &pf;
  z3::context &cxt;
  z3::expr_vector &exprVec;

  int dropletStartSequenceNum;
  int mixerStartSequenceNum;
  int detectorStartSequenceNum;
  int dispenserStartSequenceNum;
  int sinkerStartSequenceNum;

  /*********************assistant function************************/
  void formulateDroplet();
  void formulateMixer();
  void formulateDetector();
  void formulateDispenser();
  void formulateSinker();
  void formulateDetecting();

  Formulator(Formulator &);
  Formulator &operator=(Formulator &);
};

} // namespace DMFB

#endif // FORMULATOR_H_