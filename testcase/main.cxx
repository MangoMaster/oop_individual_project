#include <cstdlib>
#include <iostream>
#include <ctime>
#include "solver.h"
#include "testcase.h"

using namespace std;
using namespace DMFB;

int main()
{
    time_t rawtime;
    time(&rawtime);
    cout << "Current local time and date: "
         << asctime(localtime(&rawtime))
         << endl;
    try
    {
        TestCase::example();
    }
    catch (z3::exception &ex) // 补充exception
    {
        cout << "unexpected error: " << ex << "\n";
    }

    return 0;
}