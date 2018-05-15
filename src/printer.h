#ifndef PRINTER_H_
#define PRINTER_H_

#include <fstream>
#include <string>
#include "profile.h"
#include "formulator.h"
#include "z3++.h"

namespace DMFB
{
/**
 * @brief print the results
 * 
 */
class Printer
{
public:
  Printer(const Profile &p, const z3::expr_vector &e, const Formulator &f, z3::context &c, z3::solver &s, z3::check_result &r);
  ~Printer(){};

  /***********************main function************************/
  void print(const std::string& printFile) const;

private:
  const Profile &pf;
  const z3::expr_vector &exprVec;
  const Formulator &formu;
  z3::context &cxt;
  z3::solver &solv;
  z3::check_result &resu;

  /**********************assistant function**************************/
  void printModel(const z3::model &mdl, std::ostream &fout) const;
  void printDropletModel(const z3::model &mdl, std::ostream &fout) const;
  void printMixerModel(const z3::model &mdl, std::ostream &fout) const;
  void printDetectorModel(const z3::model &mdl, std::ostream &fout) const;
  void printDispenserModel(const z3::model &mdl, std::ostream &fout) const;
  void printSinkerModel(const z3::model &mdl, std::ostream &fout) const;
  void printDetectingModel(const z3::model &mdl, std::ostream &fout) const;
  void printProfile(std::ostream &fout) const;
};

} // namespace DMFB

#endif // PRINTER_H_