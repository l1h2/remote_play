#include "stun_client.hpp"

#include <iostream>
#include <random>

#include "stun_response_validator.hpp"

using namespace StunConstants;

StunClient::StunClient(boost::asio::io_context& io_context,
                       const uint16_t local_port, const std::string& server,
                       const uint16_t server_port)
    : io_context_(io_context),
      stun_socket_(io_context, udp::endpoint(udp::v4(), local_port)),
      stun_server_(*udp::resolver(io_context)
                        .resolve(udp::v4(), server, std::to_string(server_port))
                        .begin()) {}

StunClient::~StunClient() {
    if (stun_socket_.is_open()) stun_socket_.close();
}

void StunClient::periodic_query_stun_server(std::function<void()> callback,
                                            const uint8_t interval) {
    query_stun_server();
    callback();

    auto timer = std::make_shared<boost::asio::steady_timer>(
        io_context_, std::chrono::seconds(interval));
    timer->async_wait(
        [timer, this, interval, callback](const boost::system::error_code& ec) {
            if (!ec) {
                periodic_query_stun_server(callback, interval);
            }
        });
}

void StunClient::query_stun_server() {
    generate_stun_request();

    stun_socket_.send_to(boost::asio::buffer(send_buf_), stun_server_);

    udp::endpoint sender_endpoint;
    size_t bytes_recvd = stun_socket_.receive_from(
        boost::asio::buffer(recv_buf_), sender_endpoint);

    if (sender_endpoint != stun_server_) {
        std::cerr << "Received response from unknown endpoint" << std::endl;
        return;
    }

    handle_stun_response(bytes_recvd);
}

void StunClient::print_public_socket() const {
    std::cout << public_ip_ << ":" << public_port_ << std::endl;
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

    const uint16_t port = xor_port ^ (MAGIC_COOKIE >> 16);
    if (!validator.validate_port(port)) return;

    const uint32_t ip = xor_ip ^ MAGIC_COOKIE;
    const std::string ip_str = std::to_string((ip >> 24) & 0xFF) + "." +
                               std::to_string((ip >> 16) & 0xFF) + "." +
                               std::to_string((ip >> 8) & 0xFF) + "." +
                               std::to_string(ip & 0xFF);
    if (!validator.validate_ip(ip_str)) return;

    public_ip_ = ip_str;
    public_port_ = port;
}
