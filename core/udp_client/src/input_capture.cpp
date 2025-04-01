#include "input_capture.hpp"

#include <iostream>

InputCapture::InputCapture(boost::asio::io_context& io_context,
                           UdpClient& client)
    : io_context_(io_context),
      client_(client),
      window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {}

void InputCapture::run() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
                continue;
            }

            if (event.type == sf::Event::KeyPressed) {
                client_.send_message(std::to_string(event.key.code));
            }
        }
    }

    stop_client();
}

void InputCapture::stop_client() { io_context_.stop(); }
