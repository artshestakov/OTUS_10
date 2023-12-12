#pragma once
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "async.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
//-----------------------------------------------------------------------------
using boost::asio::ip::tcp;
//-----------------------------------------------------------------------------
class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::io_service& ios, unsigned int bulk_size);
    ~Session();

    tcp::socket& get_socket();
    void start_async_read();

private:
    void handle_read(std::shared_ptr<Session>& s, const boost::system::error_code& e, size_t bytes);

private:
    tcp::socket m_Socket;
    std::vector<char> m_Data;
    async::handle_t m_AsyncHandle;
};
//-----------------------------------------------------------------------------
