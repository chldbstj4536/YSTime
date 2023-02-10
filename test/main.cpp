#include <iostream>
#include <format>
#include <ysStopwatch.hpp>
#include <ysTimer.hpp>

using namespace std;
using namespace std::chrono;
using namespace YS;
using namespace YS::Time;

void Print()
{
    cout << "Print\n";
}

int main()
{
    Timer timer(2.5s);
    Timer timer2(10s);
    Stopwatch sw;

    timer.OnTimerDone += []() { cout << "Timer1 Done!!!\n"; };
    timer2.OnTimerDone += []() { cout << "Timer2 Done!!!\n"; };

    Event<void()> e;
    e += []() { cout << "Stopwatch!!\n"; };
    sw.AddEvent(1s, e);
    sw.AddEvent(4s, e);
    sw.AddEvent(2s, e);
    sw.AddEvent(3s, e);
    sw.AddEvent(12s, e);
    sw.AddEvent(7s, e);

    sw.Start();
    timer.Start();
    timer2.Start();

    this_thread::sleep_for(15s);

    cout << "end!\n";
}