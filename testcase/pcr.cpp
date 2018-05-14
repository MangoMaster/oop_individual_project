#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::examplePcr()
{
    cout << "Solving PCR example..." << endl;

    Profile p;

    Droplet droplet1("Tris-HCL (PH 8.3)");
    p.addDroplet(droplet1);
    vector<Droplet> droplet1Vec;
    droplet1Vec.push_back(droplet1);
    p.addDispenser(droplet1Vec);

    Droplet droplet2("KCL");
    p.addDroplet(droplet2);
    vector<Droplet> droplet2Vec;
    droplet2Vec.push_back(droplet2);
    p.addDispenser(droplet2Vec);

    Mixer mixer1(droplet1, droplet2, 1, 2, 3, "mixer1");
    p.addMixer(mixer1);

    Droplet droplet3("Bovine serum albumin");
    p.addDroplet(droplet3);
    vector<Droplet> droplet3Vec;
    droplet3Vec.push_back(droplet3);
    p.addDispenser(droplet3Vec);

    Droplet droplet4("Gelatin");
    p.addDroplet(droplet4);
    vector<Droplet> droplet4Vec;
    droplet4Vec.push_back(droplet4);
    p.addDispenser(droplet4Vec);

    Mixer mixer2(droplet3, droplet4, 1, 2, 3, "mixer2");
    p.addMixer(mixer2);

    Mixer mixer3(mixer1, mixer2, 1, 2, 3, "mixer3");
    p.addMixer(mixer3);

    Droplet droplet5("Primer");
    p.addDroplet(droplet5);
    vector<Droplet> droplet5Vec;
    droplet5Vec.push_back(droplet5);
    p.addDispenser(droplet5Vec);

    Droplet droplet6("Beosynucleotide triphosphate");
    p.addDroplet(droplet6);
    vector<Droplet> droplet6Vec;
    droplet6Vec.push_back(droplet6);
    p.addDispenser(droplet6Vec);

    Mixer mixer4(droplet5, droplet6, 1, 2, 3, "mixer4");
    p.addMixer(mixer4);

    Droplet droplet7("AmpliTag DNA");
    p.addDroplet(droplet7);
    vector<Droplet> droplet7Vec;
    droplet7Vec.push_back(droplet7);
    p.addDispenser(droplet7Vec);

    Droplet droplet8("LambdaDNA");
    p.addDroplet(droplet8);
    vector<Droplet> droplet8Vec;
    droplet8Vec.push_back(droplet8);
    p.addDispenser(droplet8Vec);

    Mixer mixer5(droplet7, droplet8, 1, 2, 3, "mixer5");
    p.addMixer(mixer5);

    Mixer mixer6(mixer4, mixer5, 1, 2, 3, "mixer6");
    p.addMixer(mixer6);

    Mixer mixer7(mixer3, mixer6, 1, 2, 3, "mixer7");
    p.addMixer(mixer7);

    p.setTime(20);

    Solver s(p);
    Timer t;
    s.setObjective("min size");
    
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_pcr.txt");
    t.print("example_pcr_time.txt");

    cout << endl;
}