#include "udp_connection.hpp"

#include <iostream>

UdpPeer::UdpPeer(boost::asio::io_context& io_context,
                 const unsigned short local_port, const std::string& peer,
                 const std::string& peer_port)
    : socket_(io_context, udp::endpoint(udp::v4(), local_port)),
      timer_(io_context),
      endpoint_(*udp::resolver(io_context)
                     .resolve(udp::v4(), peer, peer_port)
                     .begin()),
      last_receive_(std::chrono::steady_clock::now()) {
    start_send();
    start_receive();
}

void UdpPeer::start_send() {
    send_time_ = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(
            send_time_ - last_receive_) > std::chrono::seconds(TIMEOUT)) {
        std::cerr << "Connection timed out." << std::endl;
        return;
    }

    send_message(PING, endpoint_);
    timer_.expires_after(std::chrono::milliseconds(PING_INTERVAL));
    timer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) start_send();
    });
}

void UdpPeer::start_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        [this](const boost::system::error_code& ec, std::size_t bytes_recvd) {
            handle_receive(ec, bytes_recvd);
        });
}

void UdpPeer::handle_receive(const boost::system::error_code& ec,
                             std::size_t bytes_recvd) {
    if (ec) {
        std::cerr << "Error: " << ec.message() << std::endl;
        return;
    }

    int message = recv_buffer_[0];
    if (validate_message(message)) handle_response(message);

    start_receive();
}

bool UdpPeer::validate_message(const int message) const {
    if (!validade_endpoint()) return false;
    if (!validate_message_type(message)) return false;

    return true;
}

bool UdpPeer::validade_endpoint() const {
    if (remote_endpoint_ != endpoint_) {
        std::cerr << "Received message from unknown endpoint: "
                  << remote_endpoint_ << std::endl;
        return false;
    }
    return true;
}

bool UdpPeer::validate_message_type(const int message) const {
    return message == PING || message == PONG;
}

void UdpPeer::handle_response(const int message) {
    switch (message) {
        case PING:
            send_message(PONG, remote_endpoint_);
            break;
        case PONG:
            handle_pong();
            break;
        default:
            std::cerr << "Unknown message: " << message << std::endl;
    }
}

void UdpPeer::handle_pong() {
    last_receive_ = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        last_receive_ - send_time_);
    std::cout << elapsed.count() << std::endl;
}

void UdpPeer::send_message(const int message, const udp::endpoint& endpoint) {
    socket_.send_to(boost::asio::buffer(&message, sizeof(message)), endpoint);
}
