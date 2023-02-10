#pragma once

#include <chrono>

namespace YS::Time
{
    class BaseTimer
    {
    public:
        enum class State { Stop, Run, Pause };

    protected:
        BaseTimer() = default;
        BaseTimer(BaseTimer const &) = default;
        BaseTimer(BaseTimer &&) = default;
        ~BaseTimer() = default;
        BaseTimer& operator=(BaseTimer const&) = default;
        BaseTimer& operator=(BaseTimer &&) = default;

    public:
        void Start();
        void Pause();
        void Stop() { m_state = State::Stop; }

        State GetState() { return m_state; }

    protected:
        std::chrono::nanoseconds GetDuration() const;

    private:
        using timepoint = std::chrono::high_resolution_clock::time_point;
        timepoint m_tpStart;
        timepoint m_tpPause;
        State m_state = State::Stop;
    };
}