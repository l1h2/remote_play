#ifndef INPUT_CAPTURE_HPP
#define INPUT_CAPTURE_HPP

#include <SFML/Window.hpp>

#include "udp_client.hpp"

const std::string WINDOW_TITLE = "Keyboard Input Capture";
const unsigned int WINDOW_WIDTH = 640;
const unsigned int WINDOW_HEIGHT = 480;

/**
 * @class InputCapture
 * @brief Captures keyboard input and sends it to the UDP client.
 */
class InputCapture {
   public:
    /**
     * @brief Construct a new InputCapture object
     *
     * @param io_context Boost ASIO context
     * @param client UDP client to send the input
     */
    InputCapture(boost::asio::io_context& io_context, UdpClient& client);

    /**
     * @brief Destroy the InputCapture object
     */
    ~InputCapture() = default;

    /**
     * @brief Render window and run the input capture loop
     */
    void run();

   private:
    void stop_client();

    boost::asio::io_context& io_context_;
    UdpClient& client_;
    sf::Window window_;
};

#endif  // INPUT_CAPTURE_HPP
