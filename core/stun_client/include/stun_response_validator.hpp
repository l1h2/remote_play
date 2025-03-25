#ifndef STUN_RESPONSE_VALIDATOR_HPP
#define STUN_RESPONSE_VALIDATOR_HPP

#include <array>
#include <cstdint>
#include <string>

/**
 * @class StunResponseValidator
 * @brief Validate a STUN response
 */
class StunResponseValidator {
   public:
    /**
     * @brief Construct a new StunResponseValidator object
     *
     * @param recv_buf Buffer with the received data
     * @param transaction_id Transaction ID to validate
     */
    StunResponseValidator(const std::array<unsigned char, 1024>& recv_buf,
                          const std::array<unsigned char, 12>& transaction_id);

    /**
     * @brief Validates a STUN response
     *
     * @param bytes_recvd Number of bytes received
     * @return true if the response is valid, false otherwise
     */
    bool validate_stun_response(const std::size_t bytes_recvd) const;

    /**
     * @brief Validates a port number
     *
     * @param port Port number to validate
     * @return true if the port number is valid, false otherwise
     */
    bool validate_port(const uint16_t port) const;

    /**
     * @brief Validates an IP address
     *
     * @param ip IP address to validate
     * @return true if the IP address is valid, false otherwise
     */
    bool validate_ip(const std::string& ip) const;

   private:
    bool validate_response_length(const std::size_t bytes_recvd) const;
    bool validate_message_type() const;
    bool validate_message_length(const std::size_t bytes_recvd) const;
    bool validate_magic_cookie() const;
    bool validate_transaction_id() const;
    bool validate_attribute_type() const;
    bool validate_attribute_length() const;
    bool validate_address_family() const;

    const std::array<unsigned char, 1024>& recv_buf_;
    const std::array<unsigned char, 12>& transaction_id_;
};

#endif  // STUN_RESPONSE_VALIDATOR_HPP
