#include "server.h"
#include "utils.h"
#include "network.h"
#include "async.h"
//-----------------------------------------------------------------------------
Server::Server(boost::asio::io_service& ios, unsigned int port, unsigned int bulk_size)
    : m_IOS(ios),
    m_Acceptor(ios, tcp::endpoint(tcp::v4(), port)),
    m_BulkSize(bulk_size)
{
    start_async_accept(std::make_shared<Session>(m_IOS, m_BulkSize));
}
//-----------------------------------------------------------------------------
Server::~Server()
{

}
//-----------------------------------------------------------------------------
void Server::handle_accept(std::shared_ptr<Session> s, const boost::system::error_code& e)
{
    if (e)
    {
        std::cout << "Can't accept connect from client: " + e.message() << std::endl;
        s.reset();
        return;
    }

    std::cout << "Accepted connection from " << network::get_socket_address(s) << std::endl;

    s->start_async_read();
    s = std::make_shared<Session>(m_IOS, m_BulkSize);
    start_async_accept(s);
}
//-----------------------------------------------------------------------------
void Server::start_async_accept(std::shared_ptr<Session> s)
{
    m_Acceptor.async_accept(s->get_socket(),
        boost::bind(&Server::handle_accept, this, s,
            boost::asio::placeholders::error));
}
//-----------------------------------------------------------------------------
