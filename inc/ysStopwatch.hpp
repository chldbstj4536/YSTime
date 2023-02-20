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
        Stopwatch() = delete;
        Stopwatch(Stopwatch const &) = delete;
        Stopwatch (Stopwatch &&) = default;
        virtual ~Stopwatch() = default;
        Stopwatch& operator=(Stopwatch const &) = default;
        Stopwatch& operator=(Stopwatch &&) = default;

        /**
         * @brief make_shared에서 접근 가능하게 하기 위한 생성자
         */
        Stopwatch(PassKey<Stopwatch>) {}

        /**
         * @brief 흐른 시간을 얻는 함수
         * 
         * @tparam _Duration 얻고자 하는 시간의 duration 타입
         * @return _Duration 흐른 시간을 _Duration타입에 담아서 반환
         */
        template<class _Duration = _CRN::milliseconds>
        _Duration GetLap() { return _CRN::duration_cast<_Duration>(GetDuration()); }

        /**
         * @brief 흐른 시간을 얻는 함수
         * 
         * @tparam _Rep tick의 숫자를 표현할 산술 타입
         * @tparam _Period tick 기간을 표현할 std::ratio 타입
         * @return _CRN::duration<_Rep, _Period> 흐른 시간을 duration<_Rep, _Period>타입에 담아서 반환
         * 
         * @warning _Rep이 정수인 경우 좁힘변환이 발생할 수 있다.
         */
        template<number _Rep, class _Period = std::ratio<1>>
        _CRN::duration<_Rep, _Period> GetLap() { return GetLap<_CRN::duration<_Rep, _Period>>(); }

        /**
         * @brief 지정한 시점에 이벤트 추가
         * 
         * 지정한 시점이 되면 호출되는 이벤트를 추가한다.
         * 
         * @tparam _Rep tick의 숫자를 표현할 산술 타입
         * @tparam _Period tick 기간을 표현할 std::ratio 타입
         * @param eventTime 이벤트를 호출할 시점
         * @param event 등록할 이벤트
         * 
         * @warning _Rep이 정수인 경우 좁힘변환이 발생할 수 있다.
         */
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
        /**
         * @brief Stopwatch 클래스를 생성하기 위한 전역 멤버 함수
         * 
         * 기반클래스인 BaseTimer가 enable_shraed_base클래스를 상속받으므로,
         * 사용자가 생성자를 통한 직접 생성은 좋은 방법이 아니다.
         * 따라서 Create 전역함수를 통한 shared_ptr에 객체를 담아서 생성해주는 함수이다.
         * 
         * @return std::shared_ptr<Stopwatch> 생성된 Stopwatch
         */
        static std::shared_ptr<Stopwatch> Create();

    private:
        /**
         * @brief 타이머에 담긴 이벤트를 관리하는 큐
         */
#define _QUEUE_TYPE std::pair<_CRN::nanoseconds, Event<void()>>
        using pqEvent = std::priority_queue<_QUEUE_TYPE, std::vector<_QUEUE_TYPE>, bool(*)(_QUEUE_TYPE, _QUEUE_TYPE)>;
        pqEvent m_eventQ { [](_QUEUE_TYPE lhs, _QUEUE_TYPE rhs) { return lhs.first > rhs.first; } };
#undef _QUEUE_TYPE
        /**
         * @brief 이벤트 큐의 데이터 레이스를 방지하기 위한 변수
         */
        std::atomic_flag m_atomFlagForEventQ;
    };
}
