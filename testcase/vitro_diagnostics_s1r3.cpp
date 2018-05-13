#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

// shown in the paper
void TestCase::exampleVitroDiagnosticsS1R3()
{
    cout << "Solving Vitro Diagnostics S1R3 example..." << endl;

    Profile p;

    Droplet droplet1("sample1_1");
    p.addDroplet(droplet1);
    Droplet droplet2("sample1_2");
    p.addDroplet(droplet2);
    Droplet droplet3("sample1_3");
    p.addDroplet(droplet3);

    vector<Droplet> droplet123Vec;
    droplet123Vec.push_back(droplet1);
    droplet123Vec.push_back(droplet2);
    droplet123Vec.push_back(droplet3);
    p.addDispenser(droplet123Vec);

    Droplet droplet7("reagent1_1");
    p.addDroplet(droplet7);
    Droplet droplet8("reagent2_1");
    p.addDroplet(droplet8);
    Droplet droplet9("reagent3_1");
    p.addDroplet(droplet9);

    vector<Droplet> droplet7Vec;
    droplet7Vec.push_back(droplet7);
    p.addDispenser(droplet7Vec);

    vector<Droplet> droplet8Vec;
    droplet8Vec.push_back(droplet8);
    p.addDispenser(droplet8Vec);

    vector<Droplet> droplet9Vec;
    droplet9Vec.push_back(droplet9);
    p.addDispenser(droplet9Vec);

    Mixer mixer1(droplet1, droplet7, 1, 2, 2, "mixer1");
    p.addMixer(mixer1);
    p.addDetector(mixer1, 3);

    Mixer mixer2(droplet2, droplet8, 1, 2, 2, "mixer2");
    p.addMixer(mixer2);
    p.addDetector(mixer2, 3);

    Mixer mixer3(droplet3, droplet9, 1, 2, 2, "mixer3");
    p.addMixer(mixer3);
    p.addDetector(mixer3, 3);

    Solver s(p);
    s.setObjective("min time");

    p.setSize(3, 3);
    s.solve();
    s.print("example_vitro_diagnostics_s1r3_size33");

    p.setSize(2, 5);
    s.solve();
    s.print("example_vitro_diagnostics_s1r3_size25");

    p.setSize(3, 4);
    s.solve();
    s.print("example_vitro_diagnostics_s1r3_size34");

    p.setSize(3, 5);
    s.solve();
    s.print("example_vitro_diagnostics_s1r3_size35");

    p.setSize(4, 4);
    s.solve();
    s.print("example_vitro_diagnostics_s1r3_size44");

    p.setSize(3, 6);
    s.solve();
    s.print("example_vitro_diagnostics_s1r3_size36");

    cout << endl;
}