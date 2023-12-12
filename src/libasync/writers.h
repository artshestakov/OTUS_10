#pragma once
//-----------------------------------------------------------------------------
#include "observer.h"
//-----------------------------------------------------------------------------
class WriterFile : public Observer
{
public:
    WriterFile(Observable* c);

    void Update(const std::string& s, const std::chrono::system_clock::time_point& t) override;
};
//-----------------------------------------------------------------------------
class WriterConsole : public Observer
{
public:
    WriterConsole(Observable* c);

    void Update(const std::string& s, const std::chrono::system_clock::time_point& t) override;
};
//-----------------------------------------------------------------------------
