#include <iostream>
#include <fstream>
#include "printer.h"
#include "z3++.h"

using namespace std;
using namespace z3;

namespace DMFB
{
void Printer::print()
{
    ofstream fout;
    fout.open("output.txt", ofstream::out | ofstream::app);

    fout << solv.check() << endl;
    if (solv.check() == sat)
    {
        fout << solv.get_model() << endl;
    }
    
    fout.close();
}

} // namespace DMFB