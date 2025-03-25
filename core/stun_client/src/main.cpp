#include <boost/asio.hpp>
#include <iostream>

#include "common.hpp"
#include "stun_client.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw std::invalid_argument("Invalid number of arguments. Usage: " +
                                        std::string(argv[0]) + " <local_port>");
        }

        if (!Common::validate_port(argv[1])) {
            throw std::invalid_argument("Invalid port number");
        }
        const uint16_t local_port = static_cast<uint16_t>(std::stoi(argv[1]));

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
