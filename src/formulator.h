#ifndef FORMULATOR_H_
#define FORMULATOR_H_

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
  Formulator(const Profile &p) : pf(p), cxt(){};
  ~Formulator(){};

  inline const z3::context &getContext() const;
  void formulate();

private:
  const Profile pf;
  z3::context cxt;

  Formulator(Formulator &);
  Formulator &operator=(Formulator &);
};

/*******************functions********************/
inline const z3::context &Formulator::getContext() const
{
  return this->cxt;
}

}; // namespace DMFB

#endif // FORMULATOR_H_
