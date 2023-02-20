#pragma once

#include <ysBaseTimer.hpp>
#include <ysEvent.hpp>
#include <queue>
#include <concepts>

#define _CRN std::chrono

namespace YS::Time
{
    class Stopwatch final : public BaseTimer
    {
    public:
        enum class State { Stop, Run, Pause };

        Stopwatch() = delete;
        Stopwatch(Stopwatch const &) = delete;
        Stopwatch (Stopwatch &&) = default;
        virtual ~Stopwatch() = default;
        Stopwatch& operator=(Stopwatch const &) = default;
        Stopwatch& operator=(Stopwatch &&) = default;

        Stopwatch(PassKey<Stopwatch>) {}

        template<class _Duration = _CRN::milliseconds>
        _Duration GetLap() { return _CRN::duration_cast<_Duration>(GetDuration()); }

        template<number _Rep, class _Period = std::ratio<1>>
        _CRN::duration<_Rep, _Period> GetLap() { return GetLap<_CRN::duration<_Rep, _Period>>(); }

        template<number _Rep = float, class _Period = std::ratio<1>>
        void AddEvent(_CRN::duration<_Rep, _Period> const &eventTime, Event<void()> const &event)
        {
            while (m_atomFlagForEventQ.test_and_set());
            m_eventQ.push({ _CRN::duration_cast<_CRN::nanoseconds>(eventTime), event});
            m_atomFlagForEventQ.clear();
        }
    private:
        virtual void OnTick() override;

    public:
        static std::shared_ptr<Stopwatch> Create();

    private:
#define _QUEUE_TYPE std::pair<_CRN::nanoseconds, Event<void()>>
        using pqEvent = std::priority_queue<_QUEUE_TYPE, std::vector<_QUEUE_TYPE>, bool(*)(_QUEUE_TYPE, _QUEUE_TYPE)>;
        pqEvent m_eventQ { [](_QUEUE_TYPE lhs, _QUEUE_TYPE rhs) { return lhs.first > rhs.first; } };
#undef _QUEUE_TYPE
        std::atomic_flag m_atomFlagForEventQ;
    };
}
