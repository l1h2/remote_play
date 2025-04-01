#include "udp_client.hpp"

#include <iostream>

UdpClient::UdpClient(boost::asio::io_context& io_context,
                     const unsigned short local_port, const std::string& server,
                     const std::string& server_port)
    : socket_(io_context, udp::endpoint(udp::v4(), local_port)),
      server_endpoint_(*udp::resolver(io_context)
                            .resolve(udp::v4(), server, server_port)
                            .begin()),
      last_pong_(std::chrono::steady_clock::now()),
      timer_(io_context) {
    start_receive();
    start_ping();
}

UdpClient::~UdpClient() {
    if (socket_.is_open()) socket_.close();
}

void UdpClient::send_message(const std::string& message) {
    socket_.async_send_to(boost::asio::buffer(message), server_endpoint_,
                          [this, message](const boost::system::error_code& ec,
                                          std::size_t /*bytes_sent*/) {
                              if (ec) {
                                  std::cerr << "Error: " << ec.message()
                                            << std::endl;
                              }
                          });
}

void UdpClient::start_receive() {
    auto remote_endpoint = std::make_shared<udp::endpoint>();
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), *remote_endpoint,
        [this, remote_endpoint](const boost::system::error_code& ec,
                                std::size_t bytes_recvd) {
            handle_receive(ec, bytes_recvd, *remote_endpoint);
        });
}

void UdpClient::handle_receive(const boost::system::error_code& ec,
                               const std::size_t bytes_recvd,
                               const udp::endpoint& remote_endpoint) {
    if (ec) {
        std::cerr << "Error: " << ec.message() << std::endl;
        return;
    }

    const std::string message(recv_buffer_.data(), bytes_recvd);
    start_receive();  // Receiving the next message after flushing the buffer

    if (validate_message(message, bytes_recvd, remote_endpoint)) {
        handle_response(message);
    }
}

bool UdpClient::validate_message(const std::string& /* message */,
                                 const std::size_t bytes_recvd,
                                 const udp::endpoint& remote_endpoint) const {
    if (!validate_endpoint(remote_endpoint)) return false;
    if (!validate_message_size(bytes_recvd)) return false;

    return true;
}

bool UdpClient::validate_endpoint(const udp::endpoint& remote_endpoint) const {
    if (remote_endpoint != server_endpoint_) {
        std::cerr << "Received message from unknown endpoint: "
                  << remote_endpoint << std::endl;
        return false;
    }
    return true;
}

bool UdpClient::validate_message_size(const std::size_t bytes_recvd) const {
    if (bytes_recvd > recv_buffer_.size()) {
        std::cerr << "Received message too large." << std::endl;
        return false;
    }

    if (bytes_recvd == 0) {
        std::cerr << "Received empty message." << std::endl;
        return false;
    }

    return true;
}

void UdpClient::handle_response(const std::string& message) {
    if (message == "pong") handle_pong();
}

void UdpClient::handle_pong() {
    last_pong_ = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        last_pong_ - ping_time_);
    std::cout << elapsed.count() << std::endl;
}

void UdpClient::start_ping() {
    ping_time_ = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(
            ping_time_ - last_pong_) > std::chrono::seconds(TIMEOUT)) {
        std::cerr << "Connection timed out." << std::endl;
        return;
    }

    send_message("ping");
    timer_.expires_after(std::chrono::milliseconds(PING_INTERVAL));
    timer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) start_ping();
    });
}
