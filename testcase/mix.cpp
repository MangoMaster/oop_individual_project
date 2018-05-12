#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::exampleMix()
{
    cout << "Solving mix example..." << endl;

    Profile p;
    Droplet droplet1("droplet1");
    p.addDroplet(droplet1);
    vector<Droplet> droplet1Vec;
    droplet1Vec.push_back(droplet1);
    p.addDispenser(droplet1Vec);

    Droplet droplet2("droplet2");
    p.addDroplet(droplet2);
    vector<Droplet> droplet2Vec;
    droplet2Vec.push_back(droplet2);
    p.addDispenser(droplet2Vec);

    Mixer mixer3(droplet1, droplet2, 2, 3, 5, "mixer3");
    p.addMixer(mixer3);
    p.addDetector(mixer3, 5);

    p.setSize(4, 5);

    Solver s(p);
    s.setObjective("min time");
    s.solve();
    s.print("example_mix.txt");

    cout << endl;
}