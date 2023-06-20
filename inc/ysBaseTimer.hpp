#pragma once

#include <chrono>
#include <forward_list>
#include <queue>
#include <thread>
#include <atomic>
#include <ysUtility.hpp>
#include <ysDefine.hpp>

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
        BaseTimer() = default;
        BaseTimer(BaseTimer const&) = default;
        BaseTimer(BaseTimer &&) = default;
        virtual ~BaseTimer() = default;
        BaseTimer& operator=(BaseTimer const&) = default;
        BaseTimer& operator=(BaseTimer &&) = default;

    protected:
        /**
         * @brief 타이머가 생성되면 호출되는 함수
         * 
         * timer 스레드가 없다면 새로 스레드를 생성한다.
         */
        virtual void Init();
    public:
        /**
         * @brief 타이머 시작
         * 
         * 타이머가 정지되어 있다면 타이머를 새로 시작한다
         * 일시정지 중이라면 다시 작동한다.
         * 이미 타이머가 작동중이면 아무일도 일어나지 않는다.
         */
        void Start() noexcept;
        /**
         * @brief 타이머 일시정지
         * 
         * 타이머가 작동중이라면 일시정지 한다.
         * 타이머가 작동중이지 않다면(Stop or Pause) 아무일도 일어나지 않는다.
         */
        void Pause() noexcept;
        /**
         * @brief 타이머 정지
         * 
         * 타이머를 정지상태로 만든다.
         */
        void Stop() noexcept { m_state = State::Stop; }

        /**
         * @brief 타이머의 상태를 얻는다.
         * 
         * @return State 현재 타이머의 상태
         */
        State GetState() const noexcept { return m_state; }

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
        std::chrono::nanoseconds GetDuration() const noexcept;

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
                // 소멸자가 호출됐다는 것은 프로그램이 종료되었다는 의미
                // 따라서 timerThread에서의 작업이 끝나는지 체크 후 request_stop 함수를 호출해 종료하도록 한다.
                while (atomFlag.test_and_set());

                if (timerThread.get_id() != std::jthread::id())
                {
                    timerThread.get_stop_source().request_stop();
                    atomFlag.clear();
                    timerThread.join();
                }
            }

            /**
             * @brief Tick을 계산하는 Timer스레드, 해당 스레드에서 OnTick 함수가 호출된다.
             */
            std::jthread timerThread;
            /**
             * @brief Thread에서 동작중인 forward_list
             */
            std::forward_list<std::shared_ptr<BaseTimer>> timerList;
            /**
             * @brief TimerMap에 들어갈 Timer들에 대한 버퍼
             */
            std::queue<std::shared_ptr<BaseTimer>> timerQ;
            /**
             * @brief TimerThread에서 데이터 레이싱을 방지하기 위한 atomic_flag변수
             */
            std::atomic_flag atomFlag;

        } ms_data;
    };
}