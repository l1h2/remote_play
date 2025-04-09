#ifndef UDP_CONNECTION_HPP
#define UDP_CONNECTION_HPP

#include <boost/asio.hpp>

using boost::asio::ip::udp;

constexpr uint16_t PING_INTERVAL = 1000;  // milliseconds
constexpr uint8_t TIMEOUT = 30;           // seconds

/**
 * @class UdpPeer
 * @brief UDP peer for sending and receiving messages
 */
class UdpPeer {
   public:
    /**
     * @brief Construct a new UdpPeer object
     *
     * @param io_context Boost ASIO context
     * @param local_port Local port to bind the UDP socket
     * @param peer Peer name or IP address
     * @param peer_port Peer port number
     */
    UdpPeer(boost::asio::io_context& io_context,
            const unsigned short local_port, const std::string& peer,
            const std::string& peer_port);

    /**
     * @brief Destroy the Udp Peer object
     */
    ~UdpPeer();

   private:
    void start_send();
    void start_receive();
    void start_listener();
    void handle_receive(const boost::system::error_code& ec,
                        const std::size_t bytes_recvd,
                        const udp::endpoint& remote_endpoint);
    void handle_input(const std::string& input);
    bool validate_message(const int message, const std::size_t bytes_recvd,
                          const udp::endpoint& remote_endpoint) const;
    bool validate_endpoint(const udp::endpoint& remote_endpoint) const;
    bool validate_message_size(const std::size_t bytes_recvd) const;
    void handle_response(const int message,
                         const udp::endpoint& remote_endpoint);
    void handle_pong();
    void handle_process_signal(int signal,
                               const udp::endpoint& remote_endpoint);
    void reset_ping(const int signal);
    void send_message(const int message, const udp::endpoint& endpoint);

    udp::socket socket_;
    udp::endpoint endpoint_;
    int message_;
    std::array<int, 1> recv_buffer_;
    std::chrono::steady_clock::time_point send_time_;
    std::chrono::steady_clock::time_point last_receive_;
    boost::asio::steady_timer timer_;
    std::thread listener_thread_;
};

#endif  // UDP_CONNECTION_HPP
