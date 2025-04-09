#ifndef STUN_CONSTANTS_HPP
#define STUN_CONSTANTS_HPP

#include <cstdint>
#include <string>

/**
 * @namespace StunServerInfo
 * @brief Default STUN server information
 */
namespace StunServerInfo {

/**
 * @brief Default STUN server provided by Google. (stun.l.google.com)
 */
constexpr const char* GOOGLE_STUN_SERVER = "stun.l.google.com";

/**
 * @brief Default STUN server IP provided by Google. (74.125.192.127)
 */
constexpr const char* GOOGLE_STUN_SERVER_IP = "74.125.192.127";

/**
 * @brief Default STUN server port provided by Google. (19302)
 */
constexpr uint16_t GOOGLE_STUN_PORT = 19302;

}  // namespace StunServerInfo

/**
 * @namespace StunConstants
 * @brief Constants used in STUN protocol
 */
namespace StunConstants {
constexpr uint16_t BINDING_REQUEST = 0x0001;
constexpr uint16_t BINDING_RESPONSE = 0x0101;
constexpr uint16_t BINDING_ERROR_RESPONSE = 0x0111;

constexpr uint32_t MAGIC_COOKIE = 0x2112A442;

constexpr uint16_t MAPPED_ADDRESS = 0x0001;
constexpr uint16_t XOR_MAPPED_ADDRESS = 0x0020;

}  // namespace StunConstants

#endif  // STUN_CONSTANTS_HPP
