#ifndef INPUT_CAPTURE_HPP
#define INPUT_CAPTURE_HPP

#include <SFML/Window.hpp>

#include "udp_client.hpp"

constexpr const char* WINDOW_TITLE = "Keyboard Input Capture";
constexpr unsigned int WINDOW_WIDTH = 640;
constexpr unsigned int WINDOW_HEIGHT = 480;

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
    void handle_event(const sf::Event& event);
    void handle_close();
    void handle_key_event(const sf::Event& event);
    void handle_joystick_button_event(const sf::Event& event);
    void handle_joystick_moved(const sf::Event& event);
    void handle_joystick_connect_event(const sf::Event& event);
    void stop_client();

    boost::asio::io_context& io_context_;
    UdpClient& client_;
    sf::Window window_;
    std::unordered_map<sf::Event::EventType,
                       std::function<void(const sf::Event&)>>
        event_handlers_;
};

#endif  // INPUT_CAPTURE_HPP
