#include <ysStopwatch.hpp>

using namespace std;
using namespace YS::Time;

void Stopwatch::OnTick()
{
    while (m_atomFlagForEventQ.test_and_set());

    while (!m_eventQ.empty())
    {
        auto& [lap, e] = m_eventQ.top();
        if (lap <= GetDuration()) { e(); m_eventQ.pop(); }
        else break;
    }

    m_atomFlagForEventQ.clear();
}

shared_ptr<Stopwatch> Stopwatch::Create()
{
    static PassKey<Stopwatch> key;

    auto ret = make_shared<Stopwatch>(key);
    ret->Init();

    return ret;
}