#include <cstdlib>
#include <iostream>
#include <ctime>
#include "solver.h"
#include "testcase.h"

using namespace std;
using namespace DMFB;

int main()
{
    freopen("console.txt", "a", stdout);

    time_t rawtime;
    time(&rawtime);
    cout << "Current local time and date: "
         << asctime(localtime(&rawtime))
         << endl;

    TestCase::example();

    return 0;
}