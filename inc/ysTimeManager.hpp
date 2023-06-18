#pragma once

#include <chrono>
#include <ysDefine.hpp>
#include <YSDesignPattern/ysSingleton.hpp>
#include "ysStopwatch.hpp"

#define _CRN std::chrono

namespace YS::Time
{
    /**
     * @brief 시간을 관리해주는 클래스
     * 
     * TImeManager가 생성된 시점을 기준으로 다양한 시간 정보 및 시간 관련 기능을 제공해주는 클래스
     */
    class TimeManager : public DesignPattern::Creational::Singleton<TimeManager>
    {
    public:
        /// @cond
        TimeManager() = delete;
        TimeManager(TimeManager const &) = delete;
        TimeManager(TimeManager &&) = delete;
        virtual ~TimeManager() = default;
        TimeManager& operator=(TimeManager const &) = delete;
        TimeManager& operator=(TimeManager &&) = delete;
        /// @endcond

        /**
         * @brief 타임매니저를 생성하는 생성자
         * 
         * Singleton패턴의 요구조건에 맞게 PassKey를 매개변수로 받는다.
         */
        TimeManager(PassKey<Singleton<TimeManager>>);

        /**
         * @brief 시간 배속 조절 함수
         * 
         * timescale만큼 시간 배속을 조절한다.
         * 변경된 timescale값은 GetGameTime() 함수에 영향을 준다.
         * 
         * @param timeScale 변경하고자 하는 시간 배속
         */
        void SetTimeScale(Float timescale)
        {
            m_termBeforeUpdate = GetGameTime<_CRN::nanoseconds>();
            m_timescale = timescale;
        }
        Float GetTimeScale() const { return m_timescale; }

        template <typename _Duration = _CRN::milliseconds>
        _Duration GetRealTime() { return m_pTimer->GetLap<_Duration>(); }

        template <number _Rep = float, typename _Period = std::milli>
        _CRN::duration<_Rep, _Period> GetRealTime() { return GetRealTime<_CRN::duration<_Rep, _Period>>(); }

        template <typename _Duration = _CRN::milliseconds>
        _Duration GetGameTime()
        {
            _CRN::nanoseconds curTime = GetRealTime<_CRN::nanoseconds>();
            m_termBeforeUpdate += _CRN::duration_cast<_CRN::nanoseconds>((curTime - m_tpUpdate) * m_timescale);
            m_tpUpdate = curTime;
            return _CRN::duration_cast<_Duration>(m_termBeforeUpdate);
        }

        template <number _Rep, typename _Period = std::milli>
        _CRN::duration<_Rep, _Period> GetGameTime() { GetGameTime<_CRN::duration<_Rep, _Period>>(); }

        template <number _Rep, typename _Period = std::milli>
        void AddEventInRealTime(_CRN::duration<_Rep, _Period> time, Event<void()> const &e)
        {
            m_pTimer->AddEvent(GetRealTime<_Rep, _Period>() + time, e);
        }
        template <number _Rep, typename _Period = std::milli>
        void AddEventInGameTime(_CRN::duration<_Rep, _Period> time, Event<void()> const &e)
        {
        }

    private:
        _CRN::nanoseconds GetTermSinceChange() { return m_pTimer->GetLap<_CRN::nanoseconds>() - m_tpUpdate; }

    private:
        /**
         * @brief 시간을 재기 위해 사용하는 Stopwatch
         */
        std::shared_ptr<Stopwatch> m_pTimer;
        /**
         * @brief 처음부터 m_tpUpdate시점까지의 시간
         * 
         * 게임 시간은 timescale에 따라 시간 흐름의 속도가 다르기 때문에
         * 실제 시간만으로는 얼마나 시간이 흘렀는지 알 수 없다.
         * 이를 위해 게임 시간에 대한 정보를 저장하기 위한 변수이다.
         */
        _CRN::nanoseconds m_termBeforeUpdate {0};
        /**
         * @brief 타임 매니저가 갱신된 시점
         * 
         * GetGameTime()가 호출될 때마다 m_tpUpdate가 갱신된다.
         */
        _CRN::nanoseconds m_tpUpdate {0};
        /**
         * @brief 게임 시간에 영향을 주는 시간 배속
         */
        Float m_timescale = 1.0f;
    };
}

#undef _CRN