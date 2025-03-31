#ifndef COMMON_HPP
#define COMMON_HPP

#include <map>
#include <string>

/**
 * @enum Messages
 * @brief Enumerates the different types of messages that can be sent between
 * peers.
 */
enum Messages {
    PING = 1,
    PONG = 2,
    ACK = 3,
    NACK = 4,
    STREAM_REQUEST = 5,
    ACK_STREAM_REQUEST = 6,
    STREAM_ACCEPT = 7,
    ACK_STREAM_ACCEPT = 8,
    STREAM_REJECT = 9,
    ACK_STREAM_REJECT = 10
};

/**
 * @brief Maps the different types of messages to their corresponding ACKs.
 */
const std::map<int, int> ACK_MAP = {
    {PING, PONG},
    {STREAM_REQUEST, ACK_STREAM_REQUEST},
    {STREAM_ACCEPT, ACK_STREAM_ACCEPT},
    {STREAM_REJECT, ACK_STREAM_REJECT},
};

/**
 * @namespace InterprocessMessages
 * @brief Contains the different types of messages that can be sent between
 * processes.
 */
namespace InterprocessMessages {

const std::string STREAM_REQUEST = "stream_request";
const std::string ACK_STREAM_REQUEST = "ack_stream_request";
const std::string STREAM_ACCEPT = "stream_accept";
const std::string ACK_STREAM_ACCEPT = "ack_stream_accept";
const std::string STREAM_REJECT = "stream_reject";
const std::string ACK_STREAM_REJECT = "ack_stream_reject";

};  // namespace InterprocessMessages

/**
 * @brief Maps the different types of messages to their corresponding strings.
 */
const std::map<int, std::string> UDP_TO_SIGNAL_MAP = {
    {STREAM_REQUEST, InterprocessMessages::STREAM_REQUEST},
    {ACK_STREAM_REQUEST, InterprocessMessages::ACK_STREAM_REQUEST},
    {STREAM_ACCEPT, InterprocessMessages::STREAM_ACCEPT},
    {ACK_STREAM_ACCEPT, InterprocessMessages::ACK_STREAM_ACCEPT},
    {STREAM_REJECT, InterprocessMessages::STREAM_REJECT},
    {ACK_STREAM_REJECT, InterprocessMessages::ACK_STREAM_REJECT},
};

/**
 * @brief Maps the different types of messages to their corresponding UDP
 * signals.
 */
const std::map<std::string, int> SIGNAL_TO_UDP_MAP = {
    {InterprocessMessages::STREAM_REQUEST, STREAM_REQUEST},
    {InterprocessMessages::ACK_STREAM_REQUEST, ACK_STREAM_REQUEST},
    {InterprocessMessages::STREAM_ACCEPT, STREAM_ACCEPT},
    {InterprocessMessages::ACK_STREAM_ACCEPT, ACK_STREAM_ACCEPT},
    {InterprocessMessages::STREAM_REJECT, STREAM_REJECT},
    {InterprocessMessages::ACK_STREAM_REJECT, ACK_STREAM_REJECT},
};

/**
 * @namespace Common
 * @brief Contains utility functions for validating IP addresses, ports, and
 * socket strings.
 */
namespace Common {

/**
 * @brief Validates an IP address.
 *
 * @param ip The IP address to validate.
 * @return true if the IP address is valid, false otherwise.
 */
bool validate_ip(const std::string& ip);

/**
 * @brief Validates a port number.
 *
 * @param port The port number to validate.
 * @return true if the port number is valid, false otherwise.
 */
bool validate_port(const int port);

/**
 * @brief Validates a port number given as a string.
 *
 * @param port_str The port number as a string to validate.
 * @return true if the port number is valid, false otherwise.
 */
bool validate_port(const std::string& port_str);

/**
 * @brief Validates a socket string in the format "IP:port".
 *
 * @param socket_str The socket string to validate.
 * @return true if the socket string is valid, false otherwise.
 */
bool validate_socket_string(const std::string& socket_str);

/**
 * @brief Extracts the IP address and port number from a socket string.
 *
 * @param socket_str The socket string in the format "IP:port".
 * @return A tuple containing the IP address and port number.
 */
std::tuple<std::string, std::string> extract_ip_port(
    const std::string& socket_str);

}  // namespace Common

#endif  // COMMON_HPP
