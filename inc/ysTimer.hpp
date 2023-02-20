#pragma once

#include <concepts>
#include <ysDefine.hpp>
#include <ysBaseTimer.hpp>
#include <ysEvent.hpp>

#define _CRN std::chrono

namespace YS::Time
{
    /**
     * @class Timer
     * @brief 일정 시간을 정하고 완료되면 이벤트를 호출하는 클래스
     */
    class Timer final : public BaseTimer
    {
    public:
        Timer() = delete;
        Timer(Timer const &) = delete;
        Timer(Timer &&) = default;
        virtual ~Timer() = default;
        Timer& operator=(Timer const &) = default;
        Timer& operator=(Timer &&) = default;

        /**
         * @brief make_shared에서 접근 가능하게 하기 위한 생성자
         */
        template<number _Rep = long long, class _Period = std::milli>
        Timer(PassKey<Timer>, _CRN::duration<_Rep, _Period> time) : m_time(_CRN::duration_cast<_CRN::nanoseconds>(time)) { }

        /**
         * @brief 남은 시간을 얻는 함수
         * 
         * @tparam _Duration 얻고자 하는 시간의 duration 타입
         * @return _Duration 남은 시간을 _Duration타입에 담아서 반환
         * @warning 타이며가 멈추면 (완료되면) 유효하지 않은 값을 반환.
         */
        template<class _Duration = _CRN::milliseconds>
        _Duration GetRemainTime() { return _CRN::duration_cast<_Duration>(m_time - GetDuration()); }

        /**
         * @brief 남은 시간을 얻는 함수
         * 
         * @tparam _Rep tick의 숫자를 표현할 산술 타입
         * @tparam _Period tick 기간을 표현할 std::ratio 타입
         * @return _CRN::duration<_Rep, _Period> 남은 시간을 duration<_Rep, _Period>타입에 담아서 반환
         * @warning 타이며가 멈추면 (완료되면) 유효하지 않은 값을 반환.
         */
        template<number _Rep, class _Period = std::ratio<1>>
        _CRN::duration<_Rep, _Period> GetRemainTime() { return GetRemainTime<_CRN::duration<_Rep, _Period>>(); }
    private:
        virtual void OnTick() override;

    public:
        /**
         * @brief Timer 클래스를 생성하기 위한 전역 멤버 함수
         * 
         * 기반클래스인 BaseTimer가 enable_shraed_base클래스를 상속받으므로,
         * 사용자가 생성자를 통한 직접 생성은 좋은 방법이 아니다.
         * 따라서 Create 전역함수를 통한 shared_ptr에 객체를 담아서 생성해주는 함수이다.
         * 
         * @return std::shared_ptr<Timer> 생성된 Timer
         */
        template<number _Rep = long long, class _Period = std::milli>
        static std::shared_ptr<Timer> Create(_CRN::duration<_Rep, _Period> time)
        {
            static PassKey<Timer> key;

            auto ret = std::make_shared<Timer>(key, time);
            ret->Init();

            return ret;
        }

    private:
        /**
         * @brief 타이머에 설정된 시간
         */
        _CRN::nanoseconds m_time;

    public:
        /**
         * @brief 타이머 완료시 호출될 이벤트
         */
        Event<void()> OnFinish;
    };
}

#undef _CRN