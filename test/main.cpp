#include <thread>
#include <iostream>
#include <ysDefine.hpp>
#include <ysStopwatch.hpp>
#include <ysTimer.hpp>

using namespace std;
using namespace YS::Time;

int main()
{
    Timer timer(2.5s);
    Stopwatch w;

    cout << "Start!!!\n\n";

    timer.Start();
    w.Start();

    this_thread::sleep_for(1s);

    cout << w.GetLap() << endl << timer.GetRemainTime() << endl;
}