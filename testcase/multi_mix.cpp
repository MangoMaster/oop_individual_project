#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::exampleMultiMix()
{
    cout << "Solving multiple mix example..." << endl;

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

    Mixer mixer1(droplet1, droplet2, 2, 3, 3, "mixer1");
    p.addMixer(mixer1);
    p.addDetector(mixer1, 4);

    Droplet droplet3("droplet3");
    p.addDroplet(droplet3);
    vector<Droplet> dropletVec3;
    dropletVec3.push_back(droplet3);
    p.addDispenser(dropletVec3);

    Droplet droplet4("droplet4");
    p.addDroplet(droplet4);
    vector<Droplet> dropletVec4;
    dropletVec4.push_back(droplet4);
    p.addDispenser(dropletVec4);

    Mixer mixer2(droplet3, droplet4, 2, 2, 3, "mixer2");
    p.addMixer(mixer2);
    p.addDetector(mixer2, 3);

    p.setTime(13);

    Solver s(p);
    Timer t;
    s.setObjective("min size");
    
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_multi_mix.txt");
    t.print("example_multi_mix_time.txt");

    cout << endl;
}