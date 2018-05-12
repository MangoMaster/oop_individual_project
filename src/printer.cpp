#include <iostream>
#include <fstream>
#include "printer.h"
#include "z3++.h"

using namespace std;
using namespace z3;

namespace DMFB
{

Printer::Printer(const Profile &p, const z3::expr_vector &e, const Formulator &f, z3::context &c, z3::solver &s)
    : pf(p), exprVec(e), formu(f), cxt(c), solv(s)
{
}

void Printer::print(const string &printFile) const
{
    ofstream fout;
    fout.open(printFile.c_str());

    fout << solv.check() << endl
         << endl;
    if (solv.check() == sat)
    {
        printProfile(fout);
        model mdl = solv.get_model();
        printModel(mdl, fout);
    }

    fout.close();
}

void Printer::printModel(const model &mdl, ostream &fout) const
{
    printDropletModel(mdl, fout);
    printMixerModel(mdl, fout);
    printDetectorModel(mdl, fout);
    printDispenserModel(mdl, fout);
    printSinkerModel(mdl, fout);
    printDetectingModel(mdl, fout);
}

void Printer::printDropletModel(const z3::model &mdl, ostream &fout) const
{
    // droplet
    fout << "droplet:" << endl;
    for (int t = 0; t < pf.getTime(); ++t)
    {
        fout << "  time_" << t << endl;
        for (int d = 0; d < pf.getDropletNum(); ++d)
            for (int p = 0; p < pf.getSize(); ++p)
            {
                int sequenceNum = formu.computeDroplet(d, p, t);
                if (mdl.eval(exprVec[sequenceNum]).get_numeral_int() == 1)
                {
                    int x, y;
                    pf.computeXY(x, y, p);
                    fout << "    number_" << d
                         << "  name_\"" << pf.getDropletVec()[d].getName() << "\""
                         << "  x_" << x << "  y_" << y
                         << endl;
                }
            }
    }
}

void Printer::printMixerModel(const z3::model &mdl, ostream &fout) const
{
    // mixer
    fout << "mixer:" << endl;
    for (int t = 0; t < pf.getTime(); ++t)
    {
        fout << "  time_" << t << endl;
        for (int n = 0; n < pf.getMixerNum(); ++n)
            for (int p = 0; p < pf.getSize(); ++p)
            {
                int sequenceNum = formu.computeMixer(n, p, t);
                if (mdl.eval(exprVec[sequenceNum]).get_numeral_int() == 1)
                {
                    int x, y;
                    pf.computeXY(x, y, p);
                    fout << "    number_" << n
                         << "  name_\"" << pf.getMixerVec()[n].getName() << "\""
                         << "  droplet_name_\"" << pf.getMixerVec()[n].getDroplet1().getName() << "\""
                         << "_\"" << pf.getMixerVec()[n].getDroplet1().getName() << "\""
                         << "  x_" << x << " y_" << y
                         << endl;
                }
            }
    }
}

void Printer::printDetectorModel(const z3::model &mdl, ostream &fout) const
{
    // detector
    fout << "detector:" << endl;
    for (int n = 0; n < pf.getDetectorNum(); ++n)
        for (int p = 0; p < pf.getSize(); ++p)
        {
            int sequenceNum = formu.computeDetector(n, p);
            if (mdl.eval(exprVec[sequenceNum]).get_numeral_int() == 1)
            {
                int x, y;
                pf.computeXY(x, y, p);
                fout << "    number_" << n
                     << "  x_" << x << "  y_" << y
                     << endl;
            }
        }
}

void Printer::printDispenserModel(const z3::model &mdl, ostream &fout) const
{
    // dispenser
    fout << "dispenser:" << endl;
    for (int n = 0; n < pf.getDispenserNum(); ++n)
        for (int e = 0; e < pf.getEdgeNum(); ++e)
        {
            int sequenceNum = formu.computeDispenser(n, e);
            if (mdl.eval(exprVec[sequenceNum]).get_numeral_int() == 1)
            {
                fout << "    number_" << n
                     << "  edg_" << e
                     << endl;
            }
        }
}

void Printer::printSinkerModel(const z3::model &mdl, ostream &fout) const
{
    // sinker
    fout << "sinker:" << endl;
    for (int e = 0; e < pf.getEdgeNum(); ++e)
    {
        int sequenceNum = formu.computeSinker(e);
        if (mdl.eval(exprVec[sequenceNum]).get_numeral_int() == 1)
        {
            fout << "    edge_" << e
                 << endl;
        }
    }
}

void Printer::printDetectingModel(const z3::model &mdl, ostream &fout) const
{
    // detecting
    fout << "detecting:" << endl;
    for (int d = 0; d < pf.getDropletNum(); ++d)
        for (int t = 0; t < pf.getTime(); ++t)
        {
            int sequenceNum = formu.computeDetecting(d, t);
            if (mdl.eval(exprVec[sequenceNum]).get_numeral_int() == 1)
            {
                fout << "    droplet_number_" << d
                     << "  time_" << t
                     << endl;
            }
        }
}

void Printer::printProfile(ostream &fout) const
{
    int xNum, yNum;
    pf.getSize(xNum, yNum);
    fout << "x_" << xNum << endl
         << "y_" << yNum << endl
         << "time_" << pf.getTime() << endl
         << endl;
}

} // namespace DMFB