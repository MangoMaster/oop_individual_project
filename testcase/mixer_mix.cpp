#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::exampleMixerMix()
{
    cout << "Solving mixer mix example..." << endl;

    Profile p;

    Droplet droplet1("droplet1");
    p.addDroplet(droplet1);
    vector<Droplet> dropletVec1;
    dropletVec1.push_back(droplet1);
    p.addDispenser(dropletVec1);

    Droplet droplet2("droplet2");
    p.addDroplet(droplet2);
    vector<Droplet> dropletVec2;
    dropletVec2.push_back(droplet2);
    p.addDispenser(dropletVec2);

    Droplet droplet3("droplet3");
    p.addDroplet(droplet3);
    vector<Droplet> dropletVec3;
    dropletVec3.push_back(droplet3);
    p.addDispenser(dropletVec3);

    Mixer mixer1(droplet1, droplet2, 2, 2, 3, "mixer1");
    p.addMixer(mixer1);
    Detector detector1(mixer1, 2);
    p.addDetector(detector1);

    Mixer mixer2(droplet3, mixer1, 4, 1, 4, "mixer2");
    p.addMixer(mixer2);
    p.addDetector(mixer2, 3);

    p.setTime(15);

    Solver s(p);
    s.setObjective("min size");
    s.solve();
    s.print("example_mixer_mix.txt");

    cout << endl;
}