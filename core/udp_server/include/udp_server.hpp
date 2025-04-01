#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <boost/asio.hpp>

using boost::asio::ip::udp;

/**
 * @class UDPServer
 * @brief UDP server for receiving messages from a client
 */
class UDPServer {
   public:
    /**
     * @brief Construct a new UDPServer object
     *
     * @param io_context Boost ASIO context
     * @param local_port Local port to bind the UDP socket
     * @param client Client name or IP address
     * @param client_port Client port number
     */
    UDPServer(boost::asio::io_context& io_context,
              const unsigned short local_port, const std::string& client,
              const std::string& client_port);

    /**
     * @brief Destroy the UDPServer object
     */
    ~UDPServer();

   private:
    void start_receive();
    void handle_receive(const boost::system::error_code& ec,
                        const std::size_t bytes_recvd,
                        const udp::endpoint& remote_endpoint);
    bool validate_message(const std::string& message,
                          const std::size_t bytes_recvd,
                          const udp::endpoint& remote_endpoint) const;
    bool validate_endpoint(const udp::endpoint& remote_endpoint) const;
    bool validate_message_size(const std::size_t bytes_recvd) const;
    void handle_response(const std::string& message);
    void handle_ping();

    udp::socket socket_;
    udp::endpoint client_endpoint_;
    std::array<char, 1024> recv_buffer_;
};

#endif  // UDP_SERVER_H
