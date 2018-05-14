#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::exampleVitroDiagnosticsS2R2()
{
    cout << "Solving Vitro Diagnostics S2R2 example..." << endl;

    Profile p;

    Droplet droplet1("sample1_1");
    p.addDroplet(droplet1);
    Droplet droplet2("sample1_2");
    p.addDroplet(droplet2);

    vector<Droplet> droplet12Vec;
    droplet12Vec.push_back(droplet1);
    droplet12Vec.push_back(droplet2);
    p.addDispenser(droplet12Vec);

    Droplet droplet4("sample2_1");
    p.addDroplet(droplet4);
    Droplet droplet5("sample2_2");
    p.addDroplet(droplet5);

    vector<Droplet> droplet45Vec;
    droplet45Vec.push_back(droplet4);
    droplet45Vec.push_back(droplet5);
    p.addDispenser(droplet45Vec);

    Droplet droplet7("reagent1_1");
    p.addDroplet(droplet7);
    Droplet droplet8("reagent2_1");
    p.addDroplet(droplet8);
    Droplet droplet10("reagent1_2");
    p.addDroplet(droplet10);
    Droplet droplet11("reagent2_2");
    p.addDroplet(droplet11);

    vector<Droplet> droplet7_10Vec;
    droplet7_10Vec.push_back(droplet7);
    droplet7_10Vec.push_back(droplet10);
    p.addDispenser(droplet7_10Vec);

    vector<Droplet> droplet8_11Vec;
    droplet8_11Vec.push_back(droplet8);
    droplet8_11Vec.push_back(droplet11);
    p.addDispenser(droplet8_11Vec);

    Mixer mixer1(droplet1, droplet7, 1, 2, 2, "mixer1");
    p.addMixer(mixer1);
    p.addDetector(mixer1, 3);

    Mixer mixer2(droplet2, droplet8, 1, 2, 2, "mixer2");
    p.addMixer(mixer2);
    p.addDetector(mixer2, 3);

    Mixer mixer4(droplet4, droplet10, 1, 2, 2, "mixer4");
    p.addMixer(mixer4);
    p.addDetector(mixer4, 3);

    Mixer mixer5(droplet5, droplet11, 1, 2, 2, "mixer5");
    p.addMixer(mixer5);
    p.addDetector(mixer5, 3);

    Solver s(p);
    s.setObjective("min time");
    Timer t;

    p.setSize(3, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size33.txt");
    t.print("example_vitro_diagnostics_s2r2_size33_time.txt");

    p.setSize(5, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size52.txt");
    t.print("example_vitro_diagnostics_s2r2_size52_time.txt");

    p.setSize(4, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size43.txt");
    t.print("example_vitro_diagnostics_s2r2_size43_time.txt");

    p.setSize(5, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size53.txt");
    t.print("example_vitro_diagnostics_s2r2_size53_time.txt");

    p.setSize(4, 4);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size44.txt");
    t.print("example_vitro_diagnostics_s2r2_size44_time.txt");

    p.setSize(6, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size63.txt");
    t.print("example_vitro_diagnostics_s2r2_size63_time.txt");

    p.setSize(5, 4);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size54.txt");
    t.print("example_vitro_diagnostics_s2r2_size54_time.txt");

    p.setSize(6, 4);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r2_size64.txt");
    t.print("example_vitro_diagnostics_s2r2_size64_time.txt");

    cout << endl;
}