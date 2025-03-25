#ifndef UDP_CONNECTION_HPP
#define UDP_CONNECTION_HPP

#include <boost/asio.hpp>

using boost::asio::ip::udp;

const uint16_t PING_INTERVAL = 1000;  // milliseconds
const uint8_t TIMEOUT = 30;           // seconds

enum Messages {
    PING = 1,
    PONG = 2,
};

class UdpPeer {
   public:
    UdpPeer(boost::asio::io_context& io_context,
            const unsigned short local_port, const std::string& peer,
            const std::string& peer_port);

   private:
    void start_send();
    void start_receive();
    void handle_receive(const boost::system::error_code& ec,
                        std::size_t bytes_recvd);
    bool validate_message(const int message) const;
    bool validade_endpoint() const;
    bool validate_message_type(const int message) const;
    void handle_response(const int message);
    void handle_pong();
    void send_message(const int message, const udp::endpoint& endpoint);

    udp::socket socket_;
    udp::endpoint endpoint_;
    udp::endpoint remote_endpoint_;
    std::array<int, 1> recv_buffer_;
    std::chrono::steady_clock::time_point send_time_;
    std::chrono::steady_clock::time_point last_receive_;
    boost::asio::steady_timer timer_;
};

#endif  // UDP_CONNECTION_HPP
