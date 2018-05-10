#include <iostream>
#include "solver.h"

using namespace std;
using namespace DMFB;

void example1()
{
    Profile c;
    Droplet droplet1("droplet1");
    c.addDroplet(droplet1);
    vector<Droplet> droplet1Vec;
    droplet1Vec.push_back(droplet1);
    c.addDispenser(droplet1Vec);
    Droplet droplet2("droplet2");
    c.addDroplet(droplet2);
    vector<Droplet> droplet2Vec;
    droplet2Vec.push_back(droplet2);
    c.addDispenser(droplet2Vec);
    Mixer mixer3(droplet1, droplet2, 2, 2, 3, "mixer3");
    c.addMixer(mixer3);
    c.addDetector(mixer3, 5);
    c.setSize(6, 8);
    c.setTime(20);
    Solver s(c);
    s.setObjective("min time");
    s.solve();
    s.print("example1.txt");
}

void example2()
{
    Profile c;
    Droplet droplet1("droplet1");
    c.addDroplet(droplet1);
    Droplet droplet2("droplet2");
    c.addDroplet(droplet2);
    vector<Droplet> dropletVec1;
    dropletVec1.push_back(droplet1);
    dropletVec1.push_back(droplet2);
    c.addDispenser(dropletVec1);
    Droplet droplet3("droplet3");
    c.addDroplet(droplet3);
    vector<Droplet> dropletVec2;
    dropletVec2.push_back(droplet3);
    c.addDispenser(dropletVec2);
    Mixer mixer1(droplet1, droplet3, 2, 3, 5);
    c.addMixer(mixer1);
    Detector detector4(mixer1, 5);
    c.addDetector(detector4);
    c.setTime(20);
    Solver s(c);
    s.setObjective("min size");
    s.solve();
    s.print("example2.txt");
}

void exampleSimple()
{
    Profile c;
    Droplet droplet1("droplet1");
    c.addDroplet(droplet1);
    vector<Droplet> dropletVec1;
    dropletVec1.push_back(droplet1);
    c.addDispenser(dropletVec1);
    c.addDetector(droplet1, 5);
    c.setSize(5, 5);
    c.setTime(20);
    Solver s(c);
    s.setObjective("prove");
    s.solve();
    s.print("exampleSimple.txt");
}

int main()
{
    exampleSimple();
    example1();
    example2();
    return 0;
}