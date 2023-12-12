#include "network.h"
//-----------------------------------------------------------------------------
std::string network::get_socket_address(std::shared_ptr<Session> s)
{
    auto rmt = s->get_socket().remote_endpoint();
    return rmt.address().to_string() + ":" + std::to_string(rmt.port());
}
//-----------------------------------------------------------------------------
