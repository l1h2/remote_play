#include "udp_server.hpp"

#include <iostream>

UDPServer::UDPServer(boost::asio::io_context& io_context,
                     const unsigned short local_port, const std::string& client,
                     const std::string& client_port)
    : socket_(io_context, udp::endpoint(udp::v4(), local_port)),
      client_endpoint_(*udp::resolver(io_context)
                            .resolve(udp::v4(), client, client_port)
                            .begin()) {
    keyboard_ = InputSimulator::create();
    start_receive();
}

UDPServer::~UDPServer() {
    if (socket_.is_open()) socket_.close();
}

void UDPServer::start_receive() {
    auto remote_endpoint = std::make_shared<udp::endpoint>();
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), *remote_endpoint,
        [this, remote_endpoint](const boost::system::error_code& ec,
                                std::size_t bytes_recvd) {
            handle_receive(ec, bytes_recvd, *remote_endpoint);
        });
}

void UDPServer::handle_receive(const boost::system::error_code& ec,
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

bool UDPServer::validate_message(const std::string& /* message */,
                                 const std::size_t bytes_recvd,
                                 const udp::endpoint& remote_endpoint) const {
    if (!validate_endpoint(remote_endpoint)) return false;
    if (!validate_message_size(bytes_recvd)) return false;

    return true;
}

bool UDPServer::validate_endpoint(const udp::endpoint& remote_endpoint) const {
    if (remote_endpoint != client_endpoint_) {
        std::cerr << "Received message from unknown endpoint: "
                  << remote_endpoint << std::endl;
        return false;
    }
    return true;
}

bool UDPServer::validate_message_size(const std::size_t bytes_recvd) const {
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

void UDPServer::handle_response(const std::string& message) {
    if (message == "ping") {
        handle_ping();
        return;
    }

    std::cout << "Received: " << message << std::endl;
    try {
        const auto input_message = InputMessages::Message::from_string(message);
        handle_input(input_message);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing message: " << e.what() << std::endl;
    }
}

void UDPServer::handle_ping() {
    const std::string response = "pong";
    socket_.async_send_to(boost::asio::buffer(response), client_endpoint_,
                          [this, response](boost::system::error_code ec,
                                           std::size_t /*bytes_sent*/) {
                              if (ec) {
                                  std::cerr << "Error: " << ec.message()
                                            << std::endl;
                              }
                          });
}

void UDPServer::handle_input(const InputMessages::Message& input_message) {
    switch (input_message.type) {
        case InputMessages::KEY_PRESSED:
            keyboard_->keydown(input_message.id);
            break;
        case InputMessages::KEY_RELEASED:
            keyboard_->keyup(input_message.id);
            break;
        default:
            std::cerr << "Unknown message type: "
                      << static_cast<int>(input_message.type) << std::endl;
    }
}
