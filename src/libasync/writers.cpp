#include "writers.h"
#include "writer_thread.h"
//-----------------------------------------------------------------------------
WriterFile::WriterFile(Observable* c)
{
    c->Subscribe(this);
}
//-----------------------------------------------------------------------------
void WriterFile::Update(const std::string& s, const std::chrono::system_clock::time_point& t)
{
    WriterThread::Instance().Add(s, t);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
WriterConsole::WriterConsole(Observable* c)
{
    c->Subscribe(this);
}
//-----------------------------------------------------------------------------
void WriterConsole::Update(const std::string& s, const std::chrono::system_clock::time_point& t)
{
    (void)t;

    static auto worker = [](const std::string& s)
    {
        std::cout << s << std::endl;
    };

    (void)std::async(std::launch::async, worker, s);
}
//-----------------------------------------------------------------------------
