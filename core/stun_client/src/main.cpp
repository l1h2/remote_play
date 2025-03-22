#include <boost/asio.hpp>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "stun_client.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw std::invalid_argument("Invalid number of arguments. Usage: " +
                                        std::string(argv[0]) + " <local_port>");
        }

        const int port = std::stoi(argv[1]);
        if (port < 0 || port > 65535) {
            throw std::out_of_range("Port number must be between 0 and 65535");
        }
        const uint16_t local_port = static_cast<uint16_t>(port);

        boost::asio::io_context io_context;
        StunClient stun_client(io_context, local_port);

        stun_client.periodic_query_stun_server(
            [&stun_client]() { stun_client.print_public_socket(); });

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
