#include <ysBaseTimer.hpp>

using namespace YS::Time;
using namespace std;
using namespace std::chrono;

size_t BaseTimer::ms_initID;
unordered_map<std::size_t, BaseTimer&> BaseTimer::ms_mapTimer;
thread BaseTimer::ms_threadTimer;
mutex BaseTimer::ms_mutexTimer;

BaseTimer::BaseTimer() : m_ID(ms_initID++)
{
    ms_mapTimer.insert({ m_ID, *this });
    if (ms_mapTimer.size() == 1)
    {
        ms_threadTimer = thread([]()
        {
            while (true)
            {
                ms_mutexTimer.lock();
                if (ms_mapTimer.size() == 0)
                    break;
                for (auto& [ID, timer] : ms_mapTimer)
                    timer.OnTick();
                ms_mutexTimer.unlock();
                this_thread::yield();
            }
            ms_mutexTimer.unlock();
        });
    }
}
BaseTimer::BaseTimer(BaseTimer const &o)
    : BaseTimer()
{
    *this = o;
}
BaseTimer::~BaseTimer()
{
    ms_mapTimer.erase(m_ID);
    ms_mutexTimer.unlock();
    if (ms_mapTimer.size() == 0)
        ms_threadTimer.join();
}
BaseTimer& BaseTimer::operator=(BaseTimer const &o)
{
    m_tpStart = o.m_tpStart;
    m_tpPause = o.m_tpPause;
    m_state = o.m_state;
    return *this;
}

void BaseTimer::Start()
{
    switch (m_state)
    {
    case State::Run:
        return;
    case State::Pause:
        m_tpStart += high_resolution_clock::now() - m_tpPause;
        break;
    case State::Stop:
        m_tpStart = high_resolution_clock::now();
        break;
    }

    m_state = State::Run;
}
void BaseTimer::Pause()
{
    switch (m_state)
    {
    case State::Run:
        m_tpPause = high_resolution_clock::now();
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
        return std::chrono::high_resolution_clock::now() - m_tpStart;
    case State::Pause:
        return std::chrono::high_resolution_clock::now() - m_tpPause;
    }
    return 0ns;
}
