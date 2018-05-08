#ifndef PRINTER_H_
#define PRINTER_H_

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
    Printer(z3::solver &s) : solv(s){};
    void print();

  private:
    z3::solver &solv;
};

} // namespace DMFB

#endif // PRINTER_H_