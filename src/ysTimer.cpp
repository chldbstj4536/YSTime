#include <ysTimer.hpp>

using namespace YS::Time;

Timer::~Timer()
{
    ms_mutexTimer.lock();
}
void Timer::OnTick()
{
    if (m_time <= GetDuration())
    {
        OnTimerDone();
        Stop();
    }
}