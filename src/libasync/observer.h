#pragma once
//-----------------------------------------------------------------------------
#include "stdafx.h"
//-----------------------------------------------------------------------------
class Observer
{
public:
    virtual ~Observer() = default;

    virtual void Update(const std::string& s, const std::chrono::system_clock::time_point& t) = 0;
};
//-----------------------------------------------------------------------------
class Observable
{
public:
    virtual ~Observable() = default;

    virtual void Subscribe(Observer* obs) = 0;
};
//-----------------------------------------------------------------------------
