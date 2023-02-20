#pragma once

#include <concepts>
#include <ysDefine.hpp>
#include <ysBaseTimer.hpp>
#include <ysEvent.hpp>

#define _CRN std::chrono


namespace YS::Time
{
    class Timer final : public BaseTimer
    {
    public:
        Timer() = delete;
        Timer(Timer const &) = delete;
        Timer(Timer &&) = default;
        virtual ~Timer() = default;
        Timer& operator=(Timer const &) = default;
        Timer& operator=(Timer &&) = default;

        template<number _Rep = long long, class _Period = std::milli>
        Timer(PassKey<Timer>, _CRN::duration<_Rep, _Period> time) : m_time(_CRN::duration_cast<_CRN::nanoseconds>(time)) { }

        template<class _Duration = _CRN::milliseconds>
        _Duration GetRemainTime() { return _CRN::duration_cast<_Duration>(m_time - GetDuration()); }

        template<number _Rep, class _Period = std::ratio<1>>
        _CRN::duration<_Rep, _Period> GetRemainTime() { return GetRemainTime<_CRN::duration<_Rep, _Period>>(); }
    private:
        virtual void OnTick() override;

    public:
        template<number _Rep = long long, class _Period = std::milli>
        static std::shared_ptr<Timer> Create(_CRN::duration<_Rep, _Period> time)
        {
            static PassKey<Timer> key;

            auto ret = std::make_shared<Timer>(key, time);
            ret->Init();

            return ret;
        }

    private:
        _CRN::nanoseconds m_time;

    public:
        Event<void()> OnFinish;
    };
}

#undef _CRN