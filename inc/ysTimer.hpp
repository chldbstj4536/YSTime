#pragma once

#include <ysBaseTimer.hpp>

namespace YS::Time
{
    class Timer : public BaseTimer
    {
    public:
        Timer() = delete;
        Timer(Timer const &) = default;
        Timer(Timer &&) = default;
        ~Timer() = default;
        Timer& operator=(Timer const &) = default;
        Timer& operator=(Timer &&) = default;

        template<class _Rep = long long, class _Period = std::milli>
        Timer(std::chrono::duration<_Rep, _Period> time) : m_time(std::chrono::duration_cast<std::chrono::nanoseconds>(time)) { }

        template<class _Rep = float, class _Period = std::ratio<1>>
        std::chrono::duration<_Rep, _Period> GetRemainTime() { return std::chrono::duration_cast<std::chrono::duration<_Rep, _Period>>(m_time - GetDuration()); }
    private:
        std::chrono::nanoseconds m_time;
    };
}