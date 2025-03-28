#include "udp_connection.hpp"

#include <iostream>

#include "common.hpp"

UdpPeer::UdpPeer(boost::asio::io_context& io_context,
                 const unsigned short local_port, const std::string& peer,
                 const std::string& peer_port)
    : socket_(io_context, udp::endpoint(udp::v4(), local_port)),
      timer_(io_context),
      endpoint_(*udp::resolver(io_context)
                     .resolve(udp::v4(), peer, peer_port)
                     .begin()),
      last_receive_(std::chrono::steady_clock::now()),
      message_(PING) {
    start_send();
    start_receive();
    start_listener();
}

UdpPeer::~UdpPeer() {
    if (socket_.is_open()) socket_.close();
    if (listener_thread_.joinable()) listener_thread_.join();
}

void UdpPeer::start_send() {
    send_time_ = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(
            send_time_ - last_receive_) > std::chrono::seconds(TIMEOUT)) {
        std::cerr << "Connection timed out." << std::endl;
        return;
    }

    send_message(message_, endpoint_);
    timer_.expires_after(std::chrono::milliseconds(PING_INTERVAL));
    timer_.async_wait([this](const boost::system::error_code& ec) {
        if (!ec) start_send();
    });
}

void UdpPeer::start_receive() {
    auto remote_endpoint = std::make_shared<udp::endpoint>();
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), *remote_endpoint,
        [this, remote_endpoint](const boost::system::error_code& ec,
                                std::size_t bytes_recvd) {
            handle_receive(ec, bytes_recvd, *remote_endpoint);
        });
}

void UdpPeer::start_listener() {
    listener_thread_ = std::thread([this]() {
        std::string input;
        while (std::getline(std::cin, input)) {
            handle_input(input);
        }
    });
}

void UdpPeer::handle_receive(const boost::system::error_code& ec,
                             const std::size_t bytes_recvd,
                             const udp::endpoint& remote_endpoint) {
    if (ec) {
        std::cerr << "Error: " << ec.message() << std::endl;
        return;
    }

    int message = recv_buffer_[0];
    if (validate_message(message, remote_endpoint)) {
        handle_response(message, remote_endpoint);
    }

    start_receive();
}

void UdpPeer::handle_input(const std::string& input) {
    if (input == InterprocessMessages::STREAM_REQUEST) {
        message_ = STREAM_REQUEST;
    } else if (input == InterprocessMessages::STREAM_ACCEPT) {
        message_ = STREAM_ACCEPT;
    } else if (input == InterprocessMessages::STREAM_REJECT) {
        message_ = STREAM_REJECT;
    } else {
        std::cerr << "Unknown command: " << input << std::endl;
        return;
    }
}

bool UdpPeer::validate_message(const int message,
                               const udp::endpoint& remote_endpoint) const {
    if (!validate_endpoint(remote_endpoint)) return false;

    return true;
}

bool UdpPeer::validate_endpoint(const udp::endpoint& remote_endpoint) const {
    if (remote_endpoint != endpoint_) {
        std::cerr << "Received message from unknown endpoint: "
                  << remote_endpoint << std::endl;
        return false;
    }
    return true;
}

void UdpPeer::handle_response(const int message,
                              const udp::endpoint& remote_endpoint) {
    switch (message) {
        case PING:
            send_message(PONG, remote_endpoint);
            break;
        case PONG:
            handle_pong();
            break;
        case STREAM_REQUEST:
            handle_process_signal(InterprocessMessages::STREAM_REQUEST,
                                  ACK_STREAM_REQUEST, remote_endpoint);
            break;
        case ACK_STREAM_REQUEST:
            reset_ping(InterprocessMessages::ACK_STREAM_REQUEST);
            break;
        case STREAM_ACCEPT:
            handle_process_signal(InterprocessMessages::STREAM_ACCEPT,
                                  ACK_STREAM_ACCEPT, remote_endpoint);
            break;
        case ACK_STREAM_ACCEPT:
            reset_ping(InterprocessMessages::ACK_STREAM_ACCEPT);
            break;
        case STREAM_REJECT:
            handle_process_signal(InterprocessMessages::STREAM_REJECT,
                                  ACK_STREAM_REJECT, remote_endpoint);
            break;
        case ACK_STREAM_REJECT:
            reset_ping(InterprocessMessages::ACK_STREAM_REJECT);
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

void UdpPeer::handle_process_signal(const std::string& signal, const int ack,
                                    const udp::endpoint& remote_endpoint) {
    send_message(ack, remote_endpoint);
    std::cout << signal << std::endl;
}

void UdpPeer::reset_ping(const std::string& signal) {
    message_ = PING;
    std::cout << signal << std::endl;
};

void UdpPeer::send_message(const int message, const udp::endpoint& endpoint) {
    socket_.send_to(boost::asio::buffer(&message, sizeof(message)), endpoint);
}
