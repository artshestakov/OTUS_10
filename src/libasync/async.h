#pragma once
//-----------------------------------------------------------------------------
#include <cstddef>
//-----------------------------------------------------------------------------
namespace async
{
    using handle_t = void*;

    handle_t connect(unsigned int bulk);
    void receive(handle_t handle, const char* data, size_t size);
    void disconnect(handle_t handle);
}
//-----------------------------------------------------------------------------
