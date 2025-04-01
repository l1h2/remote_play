#include <iostream>

#include "common.hpp"
#include "input_capture.hpp"
#include "udp_client.hpp"

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
        UdpClient client(io_context, local_port, peer, peer_port);
        InputCapture input_capture(io_context, client);

        std::thread networking_thread([&io_context]() { io_context.run(); });
        input_capture.run();

        networking_thread.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
