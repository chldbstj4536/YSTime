#pragma once

#include <chrono>
#include <unordered_map>
#include <queue>
#include <thread>
#include <atomic>
#include <ysUtility.hpp>

namespace YS::Time
{
    /**
     * @class BaseTimer
     * @brief 모든 타이머들의 기반이 되는 클래스
     * 
     * enable_shared_from_base를 상속받기 때문에 shared_ptr을 통해 생성해야 안전하다.
     * 따라서 생성자는 protected로 접근제한이 되어있고, BaseTimer 클래스를 상속받는 객체들은
     * Create 함수를 통해 생성하도록 제한된다.
     */
    class BaseTimer : public enable_shared_from_base<BaseTimer>
    {
    public:
        enum class State { Stop, Run, Pause };

    protected:
        BaseTimer() : m_id(ms_data.initID++) {}
        BaseTimer(BaseTimer const&) = delete;
        BaseTimer(BaseTimer &&) = default;
        virtual ~BaseTimer() = default;
        BaseTimer& operator=(BaseTimer const&);
        BaseTimer& operator=(BaseTimer &&) = default;

        virtual void Init();
    public:
        /**
         * @brief 타이머 시작
         * 
         * 타이머가 정지되어 있다면 타이머를 새로 시작한다
         * 일시정지 중이라면 다시 작동한다.
         * 이미 타이머가 작동중이면 아무일도 일어나지 않는다.
         */
        void Start();
        /**
         * @brief 타이머 일시정지
         * 
         * 타이머가 작동중이라면 일시정지 한다.
         * 타이머가 작동중이지 않다면(Stop or Pause) 아무일도 일어나지 않는다.
         */
        void Pause();
        /**
         * @brief 타이머 정지
         * 
         * 타이머를 정지상태로 만든다.
         */
        void Stop() { m_state = State::Stop; }

        /**
         * @brief 타이머의 상태를 얻는다.
         * 
         * @return State 현재 타이머의 상태
         */
        State GetState() const { return m_state; }

    protected:
        /**
         * @brief 매 타이머 루프마다 호출되는 함수
         */
        virtual void OnTick() = 0;
        /**
         * @brief 타이머 시작을 기준으로 흐른 시간을 얻는다.
         * 
         * 타이머가 정지되었다면 0ns를 반환한다.
         * 
         * @return std::chrono::nanoseconds 타이머 흐른 시간
         */
        std::chrono::nanoseconds GetDuration() const;

    private:
        /**
         * @brief 타이머 스레드 시작 함수
         * 
         * 하나 이상의 타이머가 존재하면 루프를 돌면서 OnTick()를 실행한다.
         * 
         * @param stoken jthread의 top_token
         */
        static void ThreadEntry(std::stop_token stoken);

    private:
        using timepoint = std::chrono::high_resolution_clock::time_point;

        timepoint m_startTp;
        timepoint m_pauseTp;
        State m_state = State::Stop;
        std::size_t m_id;

        /**
         * @class staticDataSet 
         * @brief 전역 데이터들을 안전하게 종료시키기 위해 만든 구조체
         * 
         * 소멸자를 통해서 삭제될때 timerThread에 request_stop요청을 함으로써
         * 안전하게 스레드를 종료하고 데이터를 파괴한다.
         */
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