#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <boost/asio.hpp>

using boost::asio::ip::udp;

const uint16_t PING_INTERVAL = 1000;  // milliseconds
const uint8_t TIMEOUT = 30;           // seconds

/**
 * @class UdpClient
 * @brief UDP client for sending and receiving messages
 */
class UdpClient {
   public:
    /**
     * @brief Construct a new UdpClient object
     *
     * @param io_context Boost ASIO context
     * @param local_port Local port to bind the UDP socket
     * @param server Server name or IP address
     * @param server_port Server port number
     */
    UdpClient(boost::asio::io_context& io_context,
              const unsigned short local_port, const std::string& server,
              const std::string& server_port);

    /**
     * @brief Destroy the Udp Client object
     *
     */
    ~UdpClient();

    /**
     * @brief Schedule a message to be sent with the event loop
     *
     * @param message Message to be sent
     */
    void send_message(const std::string& message);

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
    void handle_pong();
    void start_ping();

    udp::socket socket_;
    udp::endpoint server_endpoint_;
    boost::asio::steady_timer timer_;
    std::array<char, 1024> recv_buffer_;
    std::chrono::steady_clock::time_point last_pong_;
    std::chrono::steady_clock::time_point ping_time_;
};

#endif  // UDP_CLIENT_HPP
