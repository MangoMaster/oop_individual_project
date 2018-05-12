#include "solver.h"
#include "testcase.h"

using namespace std;
using namespace DMFB;

// shown in the paper
void TestCase::exampleVitroDiagnostics()
{
    Profile p;

    Droplet droplet1("Plasma sample");
    p.addDroplet(droplet1);
    Droplet droplet2("Plasma sample");
    p.addDroplet(droplet2);
    Droplet droplet3("Plasma sample");
    p.addDroplet(droplet3);

    vector<Droplet> droplet123Vec;
    droplet123Vec.push_back(droplet1);
    droplet123Vec.push_back(droplet2);
    droplet123Vec.push_back(droplet3);
    p.addDispenser(droplet123Vec);

    Droplet droplet4("Serum sample");
    p.addDroplet(droplet4);
    Droplet droplet5("Serum sample");
    p.addDroplet(droplet5);
    Droplet droplet6("Serum sample");
    p.addDroplet(droplet6);

    vector<Droplet> droplet456Vec;
    droplet456Vec.push_back(droplet4);
    droplet456Vec.push_back(droplet5);
    droplet456Vec.push_back(droplet6);
    p.addDispenser(droplet456Vec);

    Droplet droplet7("Glucose assay reagent");
    p.addDroplet(droplet7);
    Droplet droplet8("lactate assay reagent");
    p.addDroplet(droplet8);
    Droplet droplet9("Pyruvate assay reagent");
    p.addDroplet(droplet9);
    Droplet droplet10("Glucose assay reagent");
    p.addDroplet(droplet10);
    Droplet droplet11("lactate assay reagent");
    p.addDroplet(droplet11);
    Droplet droplet12("Pyruvate assay reagent");
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

    Mixer mixer1(droplet1, droplet7, 2, 4, 5, "mixer1");
    p.addMixer(mixer1);
    p.addDetector(mixer1, 10);

    Mixer mixer2(droplet2, droplet8, 2, 4, 5, "mixer2");
    p.addMixer(mixer2);
    p.addDetector(mixer2, 8);

    Mixer mixer3(droplet3, droplet9, 2, 4, 5, "mixer3");
    p.addMixer(mixer3);
    p.addDetector(mixer3, 13);

    Mixer mixer4(droplet4, droplet10, 2, 4, 5, "mixer4");
    p.addMixer(mixer4);
    p.addDetector(mixer4, 10);

    Mixer mixer5(droplet5, droplet11, 2, 4, 5, "mixer5");
    p.addMixer(mixer5);
    p.addDetector(mixer5, 8);

    Mixer mixer6(droplet6, droplet12, 2, 4, 5, "mixer6");
    p.addMixer(mixer6);
    p.addDetector(mixer6, 13);

    p.setSize(3, 6);

    Solver s(p);
    s.setObjective("min time");
    s.solve();
    s.print("example_vitro_diagnostics");
}