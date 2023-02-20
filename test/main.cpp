#include <iostream>
#include <format>
#include <ysStopwatch.hpp>
#include <ysTimer.hpp>
#include <ysTimeManager.hpp>

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
    auto tm = TimeManager::GetInstance();
    {
        auto timer = Timer::Create(2.5s);
        auto sw = Stopwatch::Create();

        timer->OnFinish += []() { cout << "Timer1 Done!!!\n"; };

        Event<void()> e;
        e += []() { cout << "Stopwatch!!\n"; };
        sw->AddEvent(1s, e);
        sw->AddEvent(4s, e);
        sw->AddEvent(2s, e);
        sw->AddEvent(3s, e);
        sw->AddEvent(5s, e);

        sw->Start();
        timer->Start();
        this_thread::sleep_for(6s);
    }

    cout << format(R"(tm->GetRealTime() : {})", tm->GetRealTime()) << endl;
    cout << format(R"(tm->GetGameTime() : {})", tm->GetGameTime()) << endl;
    cout << format(R"(tm->SetTimeScale(5.0f))") << endl;
    tm->SetTimeScale(5.0f);

    cout << "block 1 end\n";

    {
        auto timer1 = Timer::Create(3s);
        timer1->OnFinish += []() { cout << "Timer1 Finishi!!\n"; };
        timer1->Start();

        auto sw = Stopwatch::Create();

        Event<void()> e;
        e += []() { cout << "Stopwatch!\n"; };

        sw->AddEvent(1s, Event<void()>([]() {cout << "stopwatch 1s!\n"; }));
        sw->AddEvent(2s, e);
        sw->AddEvent(3s, e);
        sw->AddEvent(4s, e);
        sw->AddEvent(5s, e);
        sw->AddEvent(6s, e);
        sw->AddEvent(7s, e);
        sw->AddEvent(8s, e);
        sw->AddEvent(9s, e);
        sw->AddEvent(10s, e);

        sw->Start();

        this_thread::sleep_for(1s);

        auto timer2 = Timer::Create(3.3s);
        auto timer3 = Timer::Create(3.5s);

        timer2->OnFinish += []() {cout << "Timer2 Finishi!!\n"; };
        timer3->OnFinish += []() {cout << "Timer3 Finishi!!\n"; };

        timer2->Start();
        timer3->Start();

        this_thread::sleep_for(1s);

        auto timer4 = Timer::Create(3.7s);
        auto timer5 = Timer::Create(3.9s);

        timer4->OnFinish += []() {cout << "Timer4 Finishi!!\n"; };
        timer5->OnFinish += []() {cout << "Timer5 Finishi!!\n"; };

        timer4->Start();
        timer5->Start();

        this_thread::sleep_for(9s);

        auto sw12 = Stopwatch::Create();
        auto sw13 = Stopwatch::Create();
        auto sw14 = Stopwatch::Create();

        sw12->Start();
        sw13->Start();
        sw14->Start();
    }

    cout << format(R"(tm->GetRealTime() : {})", tm->GetRealTime()) << endl;
    cout << format(R"(tm->GetGameTime() : {})", tm->GetGameTime()) << endl;

    cout << "end!\n";
}