#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include "solver.h"
#include "timer.h"

class TestCase
{
public:
  static void example();
  static void exampleDetect();
  static void exampleMix();
  static void exampleMultiMix();
  static void exampleMixerMix();
  static void exampleSinker();
  static void exampleVitroDiagnostics();
  static void exampleVitroDiagnosticsS1R2();
  static void exampleVitroDiagnosticsS1R3();
  static void exampleVitroDiagnosticsS2R1();
  static void exampleVitroDiagnosticsS2R2();
  static void exampleVitroDiagnosticsS2R3();
  static void examplePcr();

private:
};

#endif // EXAMPLE_H_