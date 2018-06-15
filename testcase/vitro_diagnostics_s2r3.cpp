#include <iostream>
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::exampleVitroDiagnosticsS2R3()
{
    cout << "Solving Vitro Diagnostics S2R3 example..." << endl;

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

    Droplet droplet4("sample2_1");
    p.addDroplet(droplet4);
    Droplet droplet5("sample2_2");
    p.addDroplet(droplet5);
    Droplet droplet6("sample2_3");
    p.addDroplet(droplet6);

    vector<Droplet> droplet456Vec;
    droplet456Vec.push_back(droplet4);
    droplet456Vec.push_back(droplet5);
    droplet456Vec.push_back(droplet6);
    p.addDispenser(droplet456Vec);

    Droplet droplet7("reagent1_1");
    p.addDroplet(droplet7);
    Droplet droplet8("reagent2_1");
    p.addDroplet(droplet8);
    Droplet droplet9("reagent3_1");
    p.addDroplet(droplet9);
    Droplet droplet10("reagent1_2");
    p.addDroplet(droplet10);
    Droplet droplet11("reagent2_2");
    p.addDroplet(droplet11);
    Droplet droplet12("reagent3_2");
    p.addDroplet(droplet12);

    vector<Droplet> droplet7_10Vec;
    droplet7_10Vec.push_back(droplet7);
    droplet7_10Vec.push_back(droplet10);
    p.addDispenser(droplet7_10Vec);

    vector<Droplet> droplet8_11Vec;
    droplet8_11Vec.push_back(droplet8);
    droplet8_11Vec.push_back(droplet11);
    p.addDispenser(droplet8_11Vec);

    vector<Droplet> droplet9_12Vec;
    droplet9_12Vec.push_back(droplet9);
    droplet9_12Vec.push_back(droplet12);
    p.addDispenser(droplet9_12Vec);

    Mixer mixer1(droplet1, droplet7, 1, 2, 2, "mixer1");
    p.addMixer(mixer1);
    p.addDetector(mixer1, 3);

    Mixer mixer2(droplet2, droplet8, 1, 2, 2, "mixer2");
    p.addMixer(mixer2);
    p.addDetector(mixer2, 3);

    Mixer mixer3(droplet3, droplet9, 1, 2, 2, "mixer3");
    p.addMixer(mixer3);
    p.addDetector(mixer3, 3);

    Mixer mixer4(droplet4, droplet10, 1, 2, 2, "mixer4");
    p.addMixer(mixer4);
    p.addDetector(mixer4, 3);

    Mixer mixer5(droplet5, droplet11, 1, 2, 2, "mixer5");
    p.addMixer(mixer5);
    p.addDetector(mixer5, 3);

    Mixer mixer6(droplet6, droplet12, 1, 2, 2, "mixer6");
    p.addMixer(mixer6);
    p.addDetector(mixer6, 3);

    Solver s(p);
    s.setObjective("min time");
    Timer t;

    // out of memory
    /*p.setSize(3, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();非常
    s.print("example_vitro_diagnostics_s2r3_size32.txt");
    t.print("example_vitro_diagnostics_s2r3_size32_time.txt");*/

    // out of memory
    /*p.setSize(4, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size42.txt");
    t.print("example_vitro_diagnostics_s2r3_size42_time.txt");*/

    // out of memory
    /*p.setSize(3, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size33.txt");
    t.print("example_vitro_diagnostics_s2r3_size33_time.txt");*/

    // out of memory
    /*p.setSize(5, 2);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size52.txt");
    t.print("example_vitro_diagnostics_s2r3_size52_time.txt");*/

    p.setSize(4, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size43.txt");
    t.print("example_vitro_diagnostics_s2r3_size43_time.txt");

    p.setSize(5, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size53.txt");
    t.print("example_vitro_diagnostics_s2r3_size53_time.txt");

    p.setSize(4, 4);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size44.txt");
    t.print("example_vitro_diagnostics_s2r3_size44_time.txt");

    p.setSize(6, 3);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size63.txt");
    t.print("example_vitro_diagnostics_s2r3_size63_time.txt");

    p.setSize(5, 4);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size54.txt");
    t.print("example_vitro_diagnostics_s2r3_size54_time.txt");

    p.setSize(6, 4);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size64.txt");
    t.print("example_vitro_diagnostics_s2r3_size64_time.txt");

    p.setSize(5, 5);
    t.setStartTime();
    s.solve();
    t.setEndTime();
    s.print("example_vitro_diagnostics_s2r3_size55.txt");
    t.print("example_vitro_diagnostics_s2r3_size55_time.txt");

    cout << endl;
}