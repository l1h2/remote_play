#include "common.hpp"

#include <regex>

namespace Common {

bool validate_ip(const std::string& ip) {
    std::regex pattern(R"(^(?:\d{1,3}\.){3}\d{1,3}$)");
    if (!std::regex_match(ip, pattern)) {
        return false;
    }

    std::stringstream ss(ip);
    std::string segment;
    while (std::getline(ss, segment, '.')) {
        int num = std::stoi(segment);
        if (num < 0 || num > 255) {
            return false;
        }
    }
    return true;
}

bool validate_port(const int port) { return port >= 0 && port <= 65535; }

bool validate_port(const std::string& port_str) {
    try {
        return validate_port(std::stoi(port_str));
    } catch (const std::invalid_argument& e) {
        return false;
    } catch (const std::out_of_range& e) {
        return false;
    }
}

bool validate_socket_string(const std::string& socket_str) {
    std::regex pattern(R"(^(?:\d{1,3}\.){3}\d{1,3}:\d{1,5}$)");
    if (!std::regex_match(socket_str, pattern)) {
        return false;
    }

    auto [ip, port] = extract_ip_port(socket_str);
    if (!validate_ip(ip) || !validate_port(port)) {
        return false;
    }

    return true;
}

std::tuple<std::string, std::string> extract_ip_port(
    const std::string& socket_str) {
    std::string ip;
    std::string port;
    std::stringstream ss(socket_str);
    std::getline(ss, ip, ':');
    ss >> port;
    return std::make_tuple(ip, port);
}

}  // namespace Common
