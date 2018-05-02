#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "formulator.h"
#include "z3++.h"

using namespace std;
using namespace z3;

namespace DMFB
{

void Formulator::formulate()
{
    int netNum = pf.getDropletVec().size() + pf.getMixerVec().size();
    int x, y;
    pf.getSize(x, y);
    int edgeNum = (2 * x + 2 * y);
    int t = pf.getTime();

    z3::expr_vector droplet(cxt);
    for (int i = 0; i < pf.getDropletVec().size(); ++i)
        for (int temp = 0; temp < pf.getDropletVec()[i].getNumber(); ++temp)
            for (int j = 0; j < x; ++j)
                for (int k = 0; k < y; ++k)
                    for (int l = 0; l < t; ++l)
                    {
                        stringstream ss;
                        ss << "droplet:number_" << droplet.size()
                            << "_net_" << i 
                            << "_position_" << j << "_" << k 
                            << "_time_" << l;
                        expr tempExpr = cxt.bool_const(ss.str().c_str());
                        droplet.push_back(tempExpr);
                    }
    for (int i = 0; i < pf.getMixerVec().size(); ++i)
        for (int temp = 0; temp < pf.getDropletVec()[i].getNumber(); ++temp)
            for (int j = 0; j < x; ++j)
                for (int k = 0; k < y; ++k)
                    for (int l = 0; l < t; ++l)
                    {
                        stringstream ss;
                        ss << "droplet_mixer:number_" << droplet.size()
                            << "_net_" << i 
                            << "_position_" << j << "_" << k 
                            << "_time_" << l;
                        expr tempExpr = cxt.bool_const(ss.str().c_str());
                        droplet.push_back(tempExpr);
                    }

    z3::expr_vector detector(cxt);
    for (int i = 0; i < netNum; ++i)
        for (int j = 0; j < x; ++j)
            for (int k = 0; k < y; ++k)
            {
                stringstream ss;
                ss << "detector:net_" << i 
                    << "_position_" << j << "_" << k;
                expr tempExpr = cxt.bool_const(ss.str().c_str());
                detector.push_back(tempExpr);
            }

    z3::expr_vector dispenser(cxt);
    for (int i = 0; i < edgeNum; ++i)
    {
        stringstream ss;
        ss << "dispenser:net_" << i;
        expr tempExpr = cxt.bool_const(ss.str().c_str());
        dispenser.push_back(tempExpr);
    }
    z3::expr_vector sinker(cxt);
    for (int i = 0; i < edgeNum; ++i)
    {
        stringstream ss;
        ss << "sinker:net_" << i;
        expr tempExpr = cxt.bool_const(ss.str().c_str());
        sinker.push_back(tempExpr);
    }
}

} // namespace DMFB