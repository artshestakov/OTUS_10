#include "writer_thread.h"
//-----------------------------------------------------------------------------
WriterThread::WriterThread()
    : m_IsRun(false)
{

}
//-----------------------------------------------------------------------------
WriterThread::~WriterThread()
{

}
//-----------------------------------------------------------------------------
WriterThread& WriterThread::Instance()
{
    static WriterThread writer_thread;
    return writer_thread;
}
//-----------------------------------------------------------------------------
bool WriterThread::IsRun() const
{
    return m_IsRun;
}
//-----------------------------------------------------------------------------
void WriterThread::Start(unsigned int thread_count)
{
    if (m_IsRun)
    {
        return;
    }

    for (unsigned int i = 0; i < thread_count; ++i)
    {
        std::thread(&WriterThread::Worker, this).detach();
    }

    m_IsRun = true;
}
//-----------------------------------------------------------------------------
void WriterThread::Add(const std::string& s, const std::chrono::system_clock::time_point& t)
{
    if (m_IsRun)
    {
        m_Queue.push({ s, t });
        m_Condition.notify_one();
    }
}
//-----------------------------------------------------------------------------
void WriterThread::Worker()
{
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    std::string thread_id = stream.str();

    std::unique_lock<std::mutex> lock(m_Mutex);

    while (true)
    {
        while (m_Queue.empty() && !m_Finished)
        {
            m_Condition.wait(lock);
        }

        Command cmd = m_Queue.front();

        auto time_sec = std::chrono::duration_cast<std::chrono::seconds>(cmd.TimePoint.time_since_epoch());

        std::string file_name = "bulk" + std::to_string(time_sec.count()) + "_" + thread_id + ".log";
        std::ofstream file(file_name);
        if (!file.is_open())
        {
            std::cout << "Can't write file " + file_name;
            break;
        }

        file << cmd.CMD;
        file.close();

        m_Queue.pop();
    }
}
//-----------------------------------------------------------------------------
