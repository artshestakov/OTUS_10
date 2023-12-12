#include "cmd.h"
#include "writer_thread.h"
#include "async.h"
#include "server.h"
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Invalid arguments! Example: bulk_server <tcp_port> <bulk_size>" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int port = 0;
    unsigned int bulk_size = 0;

    try
    {
        port = std::stoi(argv[1]);
        bulk_size = std::stoi(argv[2]);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    boost::asio::io_service ios;
    Server s(ios, port, bulk_size);
    ios.run();

    return EXIT_SUCCESS;
}
//-----------------------------------------------------------------------------
