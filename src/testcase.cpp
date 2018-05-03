#include <iostream>
#include "solver.h"

using namespace std;
using namespace DMFB;

void example1()
{
    Configuration cfg;
    //cfg.set("neighbor_5", true);
    Profile c(cfg);
    c.addDroplet(2);
    Droplet droplet2(3);
    c.addDroplet(droplet2);
    c.addDetector(droplet2, 10);
    c.setSize(3, 5);
    c.setTime(10);
    Solver s(c);
    s.setObjective("prove");
    s.solve();
    //cout << s.check() << endl;
}

void example2()
{
    Profile c;
    Droplet droplet1("droplet1");
    c.addDroplet(droplet1);
    c.addDroplet("droplet2", 2);
    Droplet droplet3("droplet3", 3);
    c.addDroplet(droplet3);
    Mixer mixer1(droplet1, droplet3, 10);
    c.addMixer(mixer1);
    Detector detector4(mixer1, 10);
    c.addDetector(detector4);
    c.setSize(3, 5);
    Solver s(c);
    s.setObjective("min time");
    s.solve();
    /*if (s.check())
    {
        s.print("output.txt");
    }*/
}

int main()
{
    example1();
    example2();
    return 0;
}