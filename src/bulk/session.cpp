#include "session.h"
#include "utils.h"
#include "network.h"
//-----------------------------------------------------------------------------
inline constexpr size_t BUFFER_SIZE = 1024;
//-----------------------------------------------------------------------------
Session::Session(boost::asio::io_service& ios, unsigned int bulk_size)
    : m_Socket(ios),
    m_AsyncHandle(nullptr)
{
    m_Data.resize(BUFFER_SIZE);
    m_AsyncHandle = async::connect(bulk_size);
}
//-----------------------------------------------------------------------------
Session::~Session()
{
    async::disconnect(m_AsyncHandle);
}
//-----------------------------------------------------------------------------
tcp::socket& Session::get_socket()
{
    return m_Socket;
}
//-----------------------------------------------------------------------------
void Session::start_async_read()
{
    m_Socket.async_read_some(
        boost::asio::buffer(m_Data, BUFFER_SIZE),
        boost::bind(&Session::handle_read, this, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}
//-----------------------------------------------------------------------------
void Session::handle_read(std::shared_ptr<Session>& s, const boost::system::error_code& e, size_t bytes)
{
    if (e)
    {
        std::string client_address = network::get_socket_address(s);

        //���������, � ����� ������ ����������
        if (e == boost::asio::error::connection_reset ||
            e == boost::asio::error::eof)
        {
            std::cout << "Disconnected " << client_address << std::endl;
        }
        else //� ��� ��� ��� ������
        {
            std::cout << "Can't read data from client " << client_address << ": " << e.message() << std::endl;
        }
        return;
    }

    //��������� ��������� ������� �� ������� � ������
    auto commands = utils::split_string(std::string(m_Data.begin(), m_Data.begin() + bytes), '\n');
    for (const auto& command : commands)
    {
        async::receive(m_AsyncHandle, command.c_str(), command.size());
    }

    //���������� ���������� �����
    async::flush(m_AsyncHandle);

    //������ ����� � ��������� ������� ������������ ������
    std::fill(m_Data.begin(), m_Data.end(), '\0');
    start_async_read();
}
//-----------------------------------------------------------------------------
