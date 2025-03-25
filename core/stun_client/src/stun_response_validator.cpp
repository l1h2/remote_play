#include "stun_response_validator.hpp"

#include <iostream>

#include "common.hpp"
#include "stun_constants.hpp"

using namespace StunConstants;

StunResponseValidator::StunResponseValidator(
    const std::array<unsigned char, 1024>& recv_buf,
    const std::array<unsigned char, 12>& transaction_id)
    : recv_buf_(recv_buf), transaction_id_(transaction_id) {}

bool StunResponseValidator::validate_stun_response(
    const std::size_t bytes_recvd) const {
    if (!validate_response_length(bytes_recvd)) return false;
    if (!validate_message_type()) return false;
    if (!validate_message_length(bytes_recvd)) return false;
    if (!validate_magic_cookie()) return false;
    if (!validate_transaction_id()) return false;
    if (!validate_attribute_type()) return false;
    if (!validate_attribute_length()) return false;
    if (!validate_address_family()) return false;

    return true;
}

bool StunResponseValidator::validate_port(const uint16_t port) const {
    if (!Common::validate_port(port)) {
        std::cerr << "Invalid port" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_ip(const std::string& ip) const {
    if (!Common::validate_ip(ip)) {
        std::cerr << "Invalid IP address" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_response_length(
    const std::size_t bytes_recvd) const {
    if (bytes_recvd < 20) {
        std::cerr << "Invalid STUN response" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_message_type() const {
    const uint16_t message_type = (recv_buf_[0] << 8) | recv_buf_[1];
    if (message_type != BINDING_RESPONSE) {
        std::cerr << "Invalid message type" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_message_length(
    const std::size_t bytes_recvd) const {
    const uint16_t length = (recv_buf_[2] << 8) | recv_buf_[3];
    if (length != bytes_recvd - 20) {
        std::cerr << "Invalid message length" << std::endl;
        return false;
    }
    if (length != 12) {  // Only XOR-MAPPED-ADDRESS supported
        std::cerr << "Invalid attributes" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_magic_cookie() const {
    const uint32_t magic_cookie = (recv_buf_[4] << 24) | (recv_buf_[5] << 16) |
                                  (recv_buf_[6] << 8) | recv_buf_[7];
    if (magic_cookie != MAGIC_COOKIE) {
        std::cerr << "Invalid magic cookie" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_transaction_id() const {
    std::array<unsigned char, 12> transaction_id;
    std::copy(recv_buf_.begin() + 8, recv_buf_.begin() + 20,
              transaction_id.begin());
    if (transaction_id != transaction_id_) {
        std::cerr << "Invalid transaction ID" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_attribute_type() const {
    const uint16_t attr_type = (recv_buf_[20] << 8) | recv_buf_[21];
    if (attr_type != XOR_MAPPED_ADDRESS) {
        std::cerr << "Invalid attribute type" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_attribute_length() const {
    const uint16_t attr_length = (recv_buf_[22] << 8) | recv_buf_[23];
    if (attr_length != 8) {  // Only IPv4 addresses supported
        std::cerr << "Invalid attribute length" << std::endl;
        return false;
    }
    return true;
}

bool StunResponseValidator::validate_address_family() const {
    const uint16_t family = (recv_buf_[24] << 8) | recv_buf_[25];
    if (family != 0x01) {  // Only IPv4 addresses supported
        std::cerr << "Invalid address family" << std::endl;
        return false;
    }
    return true;
}
