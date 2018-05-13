#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

// shown in the paper
void TestCase::exampleVitroDiagnosticsS1R2()
{
    cout << "Solving Vitro Diagnostics S1R2 example..." << endl;

    Profile p;

    Droplet droplet1("sample1_1");
    p.addDroplet(droplet1);
    Droplet droplet2("sample1_2");
    p.addDroplet(droplet2);

    vector<Droplet> droplet12Vec;
    droplet12Vec.push_back(droplet1);
    droplet12Vec.push_back(droplet2);
    p.addDispenser(droplet12Vec);

    Droplet droplet7("reagent1_1");
    p.addDroplet(droplet7);
    Droplet droplet8("reagent2_1");
    p.addDroplet(droplet8);

    vector<Droplet> droplet7Vec;
    droplet7Vec.push_back(droplet7);
    p.addDispenser(droplet7Vec);

    vector<Droplet> droplet8Vec;
    droplet8Vec.push_back(droplet8);
    p.addDispenser(droplet8Vec);

    Mixer mixer1(droplet1, droplet7, 1, 2, 2, "mixer1");
    p.addMixer(mixer1);
    p.addDetector(mixer1, 3);

    Mixer mixer2(droplet2, droplet8, 1, 2, 2, "mixer2");
    p.addMixer(mixer2);
    p.addDetector(mixer2, 3);

    Solver s(p);
    s.setObjective("min time");

    p.setSize(2, 4);
    s.solve();
    s.print("example_vitro_diagnostics_s1r2_size24");
    
    p.setSize(3, 3);
    s.solve();
    s.print("example_vitro_diagnostics_s1r2_size33");
    
    p.setSize(2, 5);
    s.solve();
    s.print("example_vitro_diagnostics_s1r2_size25");

    p.setSize(3, 4);
    s.solve();
    s.print("example_vitro_diagnostics_s1r2_size34");

    p.setSize(3, 5);
    s.solve();
    s.print("example_vitro_diagnostics_s1r2_size35");

    p.setSize(4, 4);
    s.solve();
    s.print("example_vitro_diagnostics_s1r2_size44");

    cout << endl;
}