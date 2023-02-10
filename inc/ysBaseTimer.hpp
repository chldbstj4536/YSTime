#pragma once

#include <chrono>
#include <unordered_map>
#include <thread>

namespace YS::Time
{
    class BaseTimer
    {
    public:
        enum class State { Stop, Run, Pause };

    protected:
        BaseTimer();
        BaseTimer(BaseTimer const &);
        BaseTimer(BaseTimer &&) = default;
        virtual ~BaseTimer();
        BaseTimer& operator=(BaseTimer const&);
        BaseTimer& operator=(BaseTimer &&) = default;

    public:
        void Start();
        void Pause();
        void Stop() { m_state = State::Stop; }

        State GetState() const { return m_state; }
        std::size_t GetID() const { return m_ID; }

    protected:
        virtual void OnTick() = 0;
        std::chrono::nanoseconds GetDuration() const;

    private:
        using timepoint = std::chrono::high_resolution_clock::time_point;
        timepoint m_tpStart;
        timepoint m_tpPause;
        State m_state = State::Stop;
        std::size_t m_ID;

        static std::size_t ms_initID;
        static std::unordered_map<std::size_t, BaseTimer&> ms_mapTimer;
        static std::thread ms_threadTimer;
    protected:
        static std::mutex ms_mutexTimer;
    };
}