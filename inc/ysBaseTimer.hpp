#pragma once

#include <chrono>
#include <unordered_map>
#include <queue>
#include <thread>
#include <atomic>
#include <ysUtility.hpp>

#include <iostream>
#include <format>

namespace YS::Time
{
    class BaseTimer : public enable_shared_from_base<BaseTimer>
    {
    public:
        enum class State { Stop, Run, Pause };

    protected:
        BaseTimer() : m_id(ms_data.initID++) { std::cout << std::format("BaseTimer() id : {}\n", m_id); }
        BaseTimer(BaseTimer const&) = delete;
        BaseTimer(BaseTimer &&) = default;
        virtual ~BaseTimer() { std::cout << std::format("~BaseTimer() id : {}\n", m_id); }
        BaseTimer& operator=(BaseTimer const&);
        BaseTimer& operator=(BaseTimer &&) = default;

        virtual void Init();
    public:
        void Start();
        void Pause();
        void Stop() { m_state = State::Stop; }

        State GetState() const { return m_state; }

    protected:
        virtual void OnTick() = 0;
        std::chrono::nanoseconds GetDuration() const;

    private:
        static void ThreadEntry(std::stop_token stoken);

    private:
        using timepoint = std::chrono::high_resolution_clock::time_point;
        timepoint m_startTp;
        timepoint m_pauseTp;
        State m_state = State::Stop;
        std::size_t m_id;

        static struct staticDataSet
        {
            ~staticDataSet()
            {
                while (atomFlag.test_and_set());

                if (timerThread.get_id() != std::jthread::id())
                {
                    timerThread.get_stop_source().request_stop();
                    atomFlag.clear();
                    timerThread.join();
                }
            }
            std::size_t initID = 0;
            std::jthread timerThread;
            std::unordered_map<std::size_t, std::shared_ptr<BaseTimer>> timerMap;
            std::queue<std::pair<std::size_t, std::shared_ptr<BaseTimer>>> timerQ;
            std::atomic_flag atomFlag;

        } ms_data;
    };
}