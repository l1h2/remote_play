#ifndef STUN_CLIENT_HPP
#define STUN_CLIENT_HPP

#include <array>
#include <boost/asio.hpp>
#include <cstdint>

#include "stun_constants.hpp"

using boost::asio::ip::udp;

/**
 * @class StunClient
 * @brief Interface to query a STUN server and get the public IP and port
 */
class StunClient {
   public:
    /**
     * @brief Construct a new StunClient object
     *
     * @param io_context Boost ASIO context
     * @param local_port Local port to bind the UDP socket
     */
    StunClient(boost::asio::io_context& io_context, uint16_t local_port);

    /**
     * @brief Periodically query a STUN server
     *
     * @param callback Function to call after each query
     * @param interval Interval in seconds between queries (default: 30)
     * @param server_name STUN server name (default: stun.l.google.com)
     * @param server_port STUN server port (default: 19302)
     */
    void periodic_query_stun_server(
        std::function<void()> callback, const uint8_t interval = 30,
        const std::string& server_name = StunServerInfo::GOOGLE_STUN_SERVER,
        const uint16_t server_port = StunServerInfo::GOOGLE_STUN_PORT);

    /**
     * @brief Query a STUN server
     *
     * @param server_name STUN server name (default: stun.l.google.com)
     * @param server_port STUN server port (default: 19302)
     */
    void query_stun_server(
        const std::string& server_name = StunServerInfo::GOOGLE_STUN_SERVER,
        const uint16_t server_port = StunServerInfo::GOOGLE_STUN_PORT);

    /**
     * @brief Get the public IP
     *
     * @return std::string Public IP
     */
    std::string public_ip() const { return public_ip_; }

    /**
     * @brief Get the public port
     *
     * @return uint16_t Public port
     */
    uint16_t public_port() const { return public_port_; }

    /**
     * @brief Print the public IP and port to stdout in the format "IP:port"
     */
    void print_public_socket() const;

   private:
    udp::endpoint resolve_endpoint(const std::string& server,
                                   const uint16_t port);
    void generate_stun_request();
    void generate_transaction_id();
    void handle_stun_response(const std::size_t bytes_recvd);

    boost::asio::io_context& io_context_;
    uint16_t local_port_;
    std::array<unsigned char, 20> send_buf_;
    std::array<unsigned char, 1024> recv_buf_;
    std::array<unsigned char, 12> transaction_id_;
    std::string public_ip_;
    uint16_t public_port_;
};

#endif  // STUN_CLIENT_HPP
