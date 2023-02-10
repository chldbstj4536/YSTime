#pragma once

#include <ysBaseTimer.hpp>

namespace YS::Time
{
    class Stopwatch : public BaseTimer
    {
    public:
        enum class State { Stop, Run, Pause };

        Stopwatch() = default;
        Stopwatch(Stopwatch const &) = default;
        Stopwatch (Stopwatch &&) = default;
        ~Stopwatch() = default;
        Stopwatch& operator=(Stopwatch const &) = default;
        Stopwatch& operator=(Stopwatch &&) = default;

        template<class _Rep = float, class _Period = std::ratio<1>>
        std::chrono::duration<_Rep, _Period> GetLap() { return std::chrono::duration_cast<std::chrono::duration<_Rep, _Period>>(GetDuration()); }
    };
}