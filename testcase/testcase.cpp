#include <iostream>
#include "solver.h"
#include "testcase.h"

using namespace std;
using namespace DMFB;

void TestCase::example()
{
    exampleDetect();
    exampleMix();
    exampleMultiMix();
    exampleMixerMix();
    exampleVitroDiagnostics();
    examplePcr();
}

void TestCase::exampleVitroDiagnostics()
{
    exampleVitroDiagnosticsS1R2();
    exampleVitroDiagnosticsS1R3();
    exampleVitroDiagnosticsS2R2();
    exampleVitroDiagnosticsS2R3();
}