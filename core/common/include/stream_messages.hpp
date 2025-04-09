#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>
#include <unordered_map>

/**
 * @namespace StreamMessages
 * @brief Contains messages used to establish a stream between peers.
 */
namespace StreamMessages {

/**
 * @enum Messages
 * @brief Enumerates the different types of messages that can be sent between
 * peers.
 */
enum Messages {
    PING = 1,
    PONG,
    ACK,
    NACK,
    STREAM_REQUEST,
    ACK_STREAM_REQUEST,
    STREAM_ACCEPT,
    ACK_STREAM_ACCEPT,
    STREAM_REJECT,
    ACK_STREAM_REJECT
};

/**
 * @brief Maps the different types of messages to their corresponding ACKs.
 */
inline const std::unordered_map<int, int> ACK_MAP = {
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

constexpr const char* STREAM_REQUEST = "stream_request";
constexpr const char* ACK_STREAM_REQUEST = "ack_stream_request";
constexpr const char* STREAM_ACCEPT = "stream_accept";
constexpr const char* ACK_STREAM_ACCEPT = "ack_stream_accept";
constexpr const char* STREAM_REJECT = "stream_reject";
constexpr const char* ACK_STREAM_REJECT = "ack_stream_reject";

};  // namespace InterprocessMessages

/**
 * @brief Maps the different types of messages to their corresponding strings.
 */
inline const std::unordered_map<Messages, std::string> UDP_TO_SIGNAL_MAP = {
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
inline const std::unordered_map<std::string, Messages> SIGNAL_TO_UDP_MAP = {
    {InterprocessMessages::STREAM_REQUEST, STREAM_REQUEST},
    {InterprocessMessages::ACK_STREAM_REQUEST, ACK_STREAM_REQUEST},
    {InterprocessMessages::STREAM_ACCEPT, STREAM_ACCEPT},
    {InterprocessMessages::ACK_STREAM_ACCEPT, ACK_STREAM_ACCEPT},
    {InterprocessMessages::STREAM_REJECT, STREAM_REJECT},
    {InterprocessMessages::ACK_STREAM_REJECT, ACK_STREAM_REJECT},
};

}  // namespace StreamMessages

#endif  // MESSAGES_HPP
