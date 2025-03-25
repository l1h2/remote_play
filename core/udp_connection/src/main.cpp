#include <iostream>

#include "common.hpp"
#include "udp_connection.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc != 4 || std::strcmp(argv[1], "-p") != 0) {
            throw std::invalid_argument(
                "Invalid arguments. Usage: " + std::string(argv[0]) +
                " -p <local_port> <peer_address> (IP:PORT)");
        }

        if (!Common::validate_port(argv[2])) {
            throw std::invalid_argument("Invalid port number");
        }

        if (!Common::validate_socket_string(argv[3])) {
            throw std::invalid_argument("Invalid peer address");
        }

        const uint16_t local_port = static_cast<uint16_t>(std::stoi(argv[2]));
        auto [peer, peer_port] = Common::extract_ip_port(argv[3]);

        boost::asio::io_context io_context;
        UdpPeer client(io_context, local_port, peer, peer_port);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
