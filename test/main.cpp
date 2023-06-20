#include <iostream>
#include <format>
#include <YSTime/ysStopwatch.hpp>
#include <YSTime/ysTimer.hpp>
#include <YSTime/ysTimeManager.hpp>

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
    cout << format("Ÿ�� �Ŵ��� �Ҵ�. UTC {}\nŸ�� �Ŵ����� ������������ �ð��� ���.\nauto tm = TimeManager::GetInstance()\n\n", system_clock::now());
    auto tm = TimeManager::GetInstance();

    cout << "Timer �׽�Ʈ\n";
    {
        cout << format(R"(auto timer = Timer::Create(2.5s))") << endl;
        auto timer = Timer::Create(2.5s);
        cout << R"(timer->OnFinish = []() { cout << format("Timeout! UTC {}\n", system_clock::now()); };)" << endl;
        timer->OnFinish = []() { cout << format("Timeout! UTC {}\n", system_clock::now()); };
        cout << format(R"(timer->Start() Sleep 3s (UTC {}))", system_clock::now()) << endl;
        timer->Start();
        this_thread::sleep_for(3s);
        cout << format(R"(Wake (UTC {}))", system_clock::now()) << endl;
    }

    cout << "\n\nStopwatch �ð� �� �̺�Ʈ ȣ�� �׽�Ʈ\n";
    {
        auto sw = Stopwatch::Create();

        Event<void()> e;
        cout << R"ys(e = []() { cout << format("1s (now UTC {})\n", system_clock::now()); };)ys" << endl;;
        cout << "sw->AddEvent(1s, e)\n";
        e = []() { cout << format("1s (now UTC {})\n", system_clock::now()); };
        sw->AddEvent(1s, e);

        cout << R"ys(e = []() { cout << format("4s (now UTC {})\n", system_clock::now()); };)ys" << endl;;
        cout << "sw->AddEvent(4s, e)\n";
        e = []() { cout << format("4s (now UTC {})\n", system_clock::now()); };
        sw->AddEvent(4s, e);

        cout << R"ys(e = []() { cout << format("2s (now UTC {})\n", system_clock::now()); };)ys" << endl;;
        cout << "sw->AddEvent(2s, e)\n";
        e = []() { cout << format("2s (now UTC {})\n", system_clock::now()); };
        sw->AddEvent(2s, e);

        cout << R"ys(e = []() { cout << format("3s (now UTC {})\n", system_clock::now()); };)ys" << endl;;
        cout << "sw->AddEvent(3s, e)\n";
        e = []() { cout << format("3s (now UTC {})\n", system_clock::now()); };
        sw->AddEvent(3s, e);

        cout << R"ys(e = []() { cout << format("5s (now UTC {})\n", system_clock::now()); };)ys" << endl;;
        cout << "sw->AddEvent(5s, e)\n";
        e = []() { cout << format("5s (now UTC {})\n", system_clock::now()); };
        sw->AddEvent(5s, e);

        cout << format("\nStopwatch Start! Sleep 6s. (current Time = UTC {})\n", system_clock::now());
        sw->Start();

        this_thread::sleep_for(6s);
        cout << format("Stopwatch End! (current Time = UTC {})\n", system_clock::now());
        sw->Start();
    }

    cout << "\n\nTimeManager�� TimeScale �׽�Ʈ\n";
    {
        cout << format("���� tm�� RealTime, GameTime\n");
        cout << format(R"(tm->GetRealTime() : {})", tm->GetRealTime()) << endl;
        cout << format(R"(tm->GetGameTime() : {})", tm->GetGameTime()) << endl << endl;

        cout << format(R"(tm->SetTimeScale(5.0f) UTC {})", system_clock::now()) << endl;
        tm->SetTimeScale(5.0f);

        cout << format(R"(Sleep 5s)", system_clock::now()) << endl;
        this_thread::sleep_for(5s);

        cout << format(R"(���� �ð� : UTC {})", system_clock::now()) << endl;
        cout << format(R"(tm->GetRealTime() : {})", tm->GetRealTime()) << endl;
        cout << format(R"(tm->GetGameTime() : {})", tm->GetGameTime()) << endl << endl;
    }
}