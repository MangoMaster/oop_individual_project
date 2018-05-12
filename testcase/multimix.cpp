#include "solver.h"
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::exampleMultiMix()
{
    Profile p;
    Droplet droplet1("droplet1");
    p.addDroplet(droplet1);
    Droplet droplet2("droplet2");
    p.addDroplet(droplet2);
    vector<Droplet> dropletVec1;
    dropletVec1.push_back(droplet1);
    dropletVec1.push_back(droplet2);
    p.addDispenser(dropletVec1);

    Droplet droplet3("droplet3");
    p.addDroplet(droplet3);
    vector<Droplet> dropletVec2;
    dropletVec2.push_back(droplet3);
    p.addDispenser(dropletVec2);

    Mixer mixer1(droplet1, droplet3, 2, 3, 5);
    p.addMixer(mixer1);
    Detector detector1(mixer1, 5);
    p.addDetector(detector1);

    Mixer mixer2(droplet2, mixer1, 2, 2, 7);
    p.addMixer(mixer2);
    p.addDetector(mixer2, 7);

    p.setTime(15);

    Solver s(p);
    s.setObjective("min size");
    s.solve();
    s.print("example_multimix.txt");
}