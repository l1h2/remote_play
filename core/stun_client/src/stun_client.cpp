#include "stun_client.hpp"

#include <chrono>
#include <iostream>
#include <random>

#include "stun_response_validator.hpp"

using namespace StunConstants;

StunClient::StunClient(boost::asio::io_context& io_context, uint16_t local_port)
    : io_context_(io_context), local_port_(local_port) {}

void StunClient::periodic_query_stun_server(std::function<void()> callback,
                                            const uint8_t interval,
                                            const std::string& server_name,
                                            const uint16_t server_port) {
    query_stun_server(server_name, server_port);
    callback();

    auto timer = std::make_shared<boost::asio::steady_timer>(
        io_context_, std::chrono::seconds(interval));
    timer->async_wait([timer, this, server_name, server_port, interval,
                       callback](const boost::system::error_code& ec) {
        if (!ec) {
            periodic_query_stun_server(callback, interval, server_name,
                                       server_port);
        }
    });
}

void StunClient::query_stun_server(const std::string& server_name,
                                   const uint16_t server_port) {
    udp::endpoint receiver_endpoint =
        resolve_endpoint(server_name, server_port);
    generate_stun_request();

    udp::socket stun_socket(io_context_, udp::endpoint(udp::v4(), local_port_));
    stun_socket.send_to(boost::asio::buffer(send_buf_), receiver_endpoint);

    udp::endpoint sender_endpoint;
    size_t bytes_recvd = stun_socket.receive_from(
        boost::asio::buffer(recv_buf_), sender_endpoint);

    stun_socket.close();

    if (bytes_recvd <= 0) {
        std::cerr << "Failed to receive STUN response" << std::endl;
        return;
    }

    handle_stun_response(bytes_recvd);
}

void StunClient::print_public_socket() const {
    std::cout << public_ip_ << ":" << public_port_ << std::endl;
}

udp::endpoint StunClient::resolve_endpoint(const std::string& server,
                                           uint16_t port) {
    udp::resolver resolver(io_context_);
    udp::resolver::results_type endpoints =
        resolver.resolve(udp::v4(), server, std::to_string(port));
    return *endpoints.begin();
}

void StunClient::generate_stun_request() {
    const uint16_t message_length = 0;

    send_buf_[0] = (BINDING_REQUEST >> 8) & 0xFF;
    send_buf_[1] = BINDING_REQUEST & 0xFF;
    send_buf_[2] = (message_length >> 8) & 0xFF;
    send_buf_[3] = message_length & 0xFF;
    send_buf_[4] = (MAGIC_COOKIE >> 24) & 0xFF;
    send_buf_[5] = (MAGIC_COOKIE >> 16) & 0xFF;
    send_buf_[6] = (MAGIC_COOKIE >> 8) & 0xFF;
    send_buf_[7] = MAGIC_COOKIE & 0xFF;

    generate_transaction_id();
    std::copy(transaction_id_.begin(), transaction_id_.end(),
              send_buf_.begin() + 8);
}

void StunClient::generate_transaction_id() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::generate(transaction_id_.begin(), transaction_id_.end(),
                  [&dis, &gen]() { return dis(gen); });
}

void StunClient::handle_stun_response(const std::size_t bytes_recvd) {
    StunResponseValidator validator(recv_buf_, transaction_id_);
    if (!validator.validate_stun_response(bytes_recvd)) return;

    const uint16_t xor_port = (recv_buf_[26] << 8) | recv_buf_[27];
    const uint32_t xor_ip = (recv_buf_[28] << 24) | (recv_buf_[29] << 16) |
                            (recv_buf_[30] << 8) | recv_buf_[31];

    public_port_ = xor_port ^ (MAGIC_COOKIE >> 16);
    const uint32_t ip = xor_ip ^ MAGIC_COOKIE;
    public_ip_ = std::to_string((ip >> 24) & 0xFF) + "." +
                 std::to_string((ip >> 16) & 0xFF) + "." +
                 std::to_string((ip >> 8) & 0xFF) + "." +
                 std::to_string(ip & 0xFF);
}
