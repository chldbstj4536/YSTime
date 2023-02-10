#include <ysBaseTimer.hpp>

using namespace YS::Time;
using namespace std::chrono;

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
    case State::Stop:
        return 0ns;
    }
}
