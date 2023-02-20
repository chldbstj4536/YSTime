#include <ysBaseTimer.hpp>
#include <iostream>

using namespace YS::Time;
using namespace std;
using namespace std::chrono;

BaseTimer::staticDataSet BaseTimer::ms_data;

BaseTimer& BaseTimer::operator=(BaseTimer const &o)
{
    m_startTp = o.m_startTp;
    m_pauseTp = o.m_pauseTp;
    m_state = o.m_state;
    return *this;
}

void BaseTimer::Init()
{
    while (ms_data.atomFlag.test_and_set());
    ms_data.timerQ.push({ m_id, shared_from_this() });
    ms_data.atomFlag.clear();
    if (ms_data.timerThread.get_id() == jthread::id())
        ms_data.timerThread = jthread(&BaseTimer::ThreadEntry);
}

void BaseTimer::Start()
{
    switch (m_state)
    {
    case State::Run:
        return;
    case State::Pause:
        m_startTp += high_resolution_clock::now() - m_pauseTp;
        break;
    case State::Stop:
        m_startTp = high_resolution_clock::now();
        break;
    }

    m_state = State::Run;
}
void BaseTimer::Pause()
{
    switch (m_state)
    {
    case State::Run:
        m_pauseTp = high_resolution_clock::now();
        m_state = State::Pause;
        break;
    case State::Pause:
    case State::Stop:
        break;
    }
}
nanoseconds BaseTimer::GetDuration() const
{
    switch (m_state)
    {
    case State::Run:
        return std::chrono::high_resolution_clock::now() - m_startTp;
    case State::Pause:
        return std::chrono::high_resolution_clock::now() - m_pauseTp;
    }
    return 0ns;
}

void BaseTimer::ThreadEntry(stop_token stoken)
{
    while (true)
    {
        while (ms_data.atomFlag.test_and_set());
        while (!ms_data.timerQ.empty()) ms_data.timerMap.insert(move(ms_data.timerQ.front())), ms_data.timerQ.pop();

        if (ms_data.timerMap.empty())
        {
            ms_data.timerThread.detach();
            ms_data.atomFlag.clear();
            return;
        }
        ms_data.atomFlag.clear();

        for (auto it = ms_data.timerMap.begin(); it != ms_data.timerMap.end();)
        {
            auto& id = it->first;
            auto& timer = it->second;

            if (timer.use_count() == 1)
            {
                ms_data.timerMap.erase(it++);
                cout << format("timer was erased. now, ms_timerMap.size is {}\n", ms_data.timerMap.size());
            }
            else
                timer->OnTick(), ++it;
        }

        if (stoken.stop_requested())
        {
            cout << "stop requested\n";
            return;
        }
        this_thread::yield();
    }
}
