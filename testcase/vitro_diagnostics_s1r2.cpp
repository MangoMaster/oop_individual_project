#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

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
    Timer t;

    p.setSize(3, 1);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size31.txt");
    t.print("example_vitro_diagnostics_s1r2_size31_time.txt");

    p.setSize(2, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size22.txt");
    t.print("example_vitro_diagnostics_s1r2_size22_time.txt");

    p.setSize(3, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size32.txt");
    t.print("example_vitro_diagnostics_s1r2_size32_time.txt");

    p.setSize(4, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size42.txt");
    t.print("example_vitro_diagnostics_s1r2_size42_time.txt");

    p.setSize(3, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size33.txt");
    t.print("example_vitro_diagnostics_s1r2_size33_time.txt");

    p.setSize(5, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size52.txt");
    t.print("example_vitro_diagnostics_s1r2_size52_time.txt");

    p.setSize(4, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size43.txt");
    t.print("example_vitro_diagnostics_s1r2_size43_time.txt");

    p.setSize(5, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size53.txt");
    t.print("example_vitro_diagnostics_s1r2_size53_time.txt");

    p.setSize(4, 4);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s1r2_size44.txt");
    t.print("example_vitro_diagnostics_s1r2_size44_time.txt");

    cout << endl;
}