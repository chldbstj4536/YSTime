#include "../inc/ysTimeManager.hpp"

using namespace std::chrono;
using namespace YS::Time;
using namespace YS::DesignPattern::Creational;

TimeManager::TimeManager(PassKey<Singleton<TimeManager>> key)
    : Singleton<TimeManager>{key}, m_pTimer(Stopwatch::Create())
{
    m_pTimer->Start();
}
