#include <ysStopwatch.hpp>

using namespace std;
using namespace std::chrono;
using namespace YS;
using namespace YS::Time;

Stopwatch::~Stopwatch()
{
    ms_mutexTimer.lock();
}

void Stopwatch::OnTick()
{
    while (!m_mapEvent.empty())
    {
        auto& [lap, e] = m_mapEvent.top();
        if (lap <= GetDuration()) { e(); m_mapEvent.pop(); }
        else break;
    }
}