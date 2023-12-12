#pragma once
//-----------------------------------------------------------------------------
#include "stdafx.h"
//-----------------------------------------------------------------------------
class WriterThread
{
private:
    struct Command
    {
        std::string CMD;
        std::chrono::system_clock::time_point TimePoint;
    };

public:
    static WriterThread& Instance();

    bool IsRun() const;
    void Start(unsigned int thread_count);
    void Add(const std::string& s, const std::chrono::system_clock::time_point& t);

private:
    void Worker();

private:
    WriterThread();
    ~WriterThread();
    WriterThread(const WriterThread&) = delete;
    WriterThread(WriterThread&&) = delete;
    WriterThread& operator=(const WriterThread&) = delete;
    WriterThread& operator=(WriterThread&&) = delete;

private:
    bool m_IsRun;
    std::atomic<bool> m_Finished;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::queue<Command> m_Queue;
};
//-----------------------------------------------------------------------------
