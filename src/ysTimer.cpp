#include "../inc/ysTimer.hpp"

using namespace YS::Time;

void Timer::OnTick()
{
    if (m_time <= GetDuration())
    {
        OnFinish();
        Stop();
    }
}
