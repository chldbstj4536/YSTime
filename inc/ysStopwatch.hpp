#pragma once

#include <ysBaseTimer.hpp>
#include <ysEvent.hpp>
#include <queue>

namespace YS::Time
{
    class Stopwatch : public BaseTimer
    {
    public:
        enum class State { Stop, Run, Pause };

        Stopwatch() = default;
        Stopwatch(Stopwatch const &) = default;
        Stopwatch (Stopwatch &&) = default;
        ~Stopwatch();
        Stopwatch& operator=(Stopwatch const &) = default;
        Stopwatch& operator=(Stopwatch &&) = default;
        
        template<class _Rep = float, class _Period = std::ratio<1>>
        std::chrono::duration<_Rep, _Period> GetLap() { return std::chrono::duration_cast<std::chrono::duration<_Rep, _Period>>(GetDuration()); }

        template<class _Rep = float, class _Period = std::ratio<1>>
        void AddEvent(std::chrono::duration<_Rep, _Period> const &eventTime, Event<void()> const &event)
        {
            m_mapEvent.push({ std::chrono::duration_cast<std::chrono::nanoseconds>(eventTime), event});
        }
    private:
        virtual void OnTick() override;
#define _QUEUE_TYPE std::pair<std::chrono::nanoseconds, Event<void()>>
        std::priority_queue<_QUEUE_TYPE, std::vector<_QUEUE_TYPE>, bool(*)(_QUEUE_TYPE, _QUEUE_TYPE)> m_mapEvent
        { [](_QUEUE_TYPE lhs, _QUEUE_TYPE rhs) { return lhs.first > rhs.first; } };
#undef _QUEUE_TYPE
    };
}