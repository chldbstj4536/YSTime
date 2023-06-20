#include <iostream>
#include "../inc/ysBaseTimer.hpp"

using namespace YS::Time;
using namespace std;
using namespace std::chrono;

BaseTimer::staticDataSet BaseTimer::ms_data;

void BaseTimer::Init()
{
    while (ms_data.atomFlag.test_and_set());

    ms_data.timerQ.push(shared_from_this());

    ms_data.atomFlag.clear();

    if (ms_data.timerThread.get_id() == jthread::id())
        ms_data.timerThread = jthread(&BaseTimer::ThreadEntry);
}

void BaseTimer::Start() noexcept
{
    switch (m_state)
    {
    case State::Run:
        return;
    case State::Pause:
        m_startTp += high_resolution_clock::now() - m_pauseTp;
        break;
    case State::Stop:
        m_startTp = high_resolution_clock::now();
        break;
    }

    m_state = State::Run;
}
void BaseTimer::Pause() noexcept
{
    switch (m_state)
    {
    case State::Run:
        m_pauseTp = high_resolution_clock::now();
        m_state = State::Pause;
        break;
    case State::Pause:
    case State::Stop:
        break;
    }
}
nanoseconds BaseTimer::GetDuration() const noexcept
{
    switch (m_state)
    {
    case State::Run:
        return std::chrono::high_resolution_clock::now() - m_startTp;
    case State::Pause:
        return std::chrono::high_resolution_clock::now() - m_pauseTp;
    }
    return 0ns;
}

void BaseTimer::ThreadEntry(stop_token stoken)
{
    while (true)
    {
        while (ms_data.atomFlag.test_and_set());

        while (!ms_data.timerQ.empty())
        {
            ms_data.timerList.push_front(move(ms_data.timerQ.front()));
            ms_data.timerQ.pop();
        }

        // atomFlag중복을 없애려고 if문 위에서 하게 되면
        // 데이터 레이싱이 발생할 수 있다. (if문 안에 들어간 후에 timerQ에 값이 들어갈 수 있음)
        if (ms_data.timerList.empty())
        {
            ms_data.timerThread.detach();
            ms_data.atomFlag.clear();
            return;
        }

        ms_data.atomFlag.clear();

        // front에 대한 처리
        while (!ms_data.timerList.empty())
        {
            if (ms_data.timerList.front().use_count() == 1)
            {
                ms_data.timerList.pop_front();
            }
            else
            {
                ms_data.timerList.front()->OnTick();
                break;
            }
        }

        if (!ms_data.timerList.empty())
        {
            // 이후 iterator들에 대한 처리
            // 첫 원소(begin)은 위에서 처리했으므로 두번째 원소부터 처리
            auto nextIter = ms_data.timerList.begin();
            auto lastIter = nextIter++;

            while (nextIter != ms_data.timerList.end())
            {
                if ((*nextIter).use_count() == 1)
                {
                    nextIter = ms_data.timerList.erase_after(lastIter);
                }
                else
                {
                    (*nextIter)->OnTick();
                    lastIter = nextIter++;
                }
            }
        }

        if (stoken.stop_requested())
        {
            cout << "stop requested\n";
            return;
        }
        this_thread::yield();
    }
}
