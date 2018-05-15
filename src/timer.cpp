#include <iostream>
#include <fstream>
#include <chrono>
#include <cassert>
#include "timer.h"

using namespace std;
using namespace chrono;

namespace DMFB
{
double Timer::getDuration() const
{
    assert(startTime < endTime);
    duration<double> duration_second(endTime - startTime);
    return duration_second.count();
}

void Timer::setStartTime()
{
    startTime = system_clock::now();
}

void Timer::setEndTime()
{
    endTime = system_clock::now();
}

void Timer::print(const std::string &printFile) const
{
    ofstream fout;
    fout.open(printFile.c_str());

    cout << "Printing into file...\t" << printFile << endl;
    fout << "total executing time: " << getDuration() << " s" << endl;

    fout.close();
}

} // namespace DMFB