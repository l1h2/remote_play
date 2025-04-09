#include "input_capture.hpp"

#include <iostream>

#include "common.hpp"

InputCapture::InputCapture(boost::asio::io_context& io_context,
                           UdpClient& client)
    : io_context_(io_context),
      client_(client),
      window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {}

void InputCapture::run() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) handle_event(event);
    }
    stop_client();
}

void InputCapture::handle_event(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            handle_close();
            break;
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
            handle_key_event(event);
            break;
        case sf::Event::JoystickButtonPressed:
        case sf::Event::JoystickButtonReleased:
            handle_joystick_button_event(event);
            break;
        case sf::Event::JoystickMoved:
            handle_joystick_moved(event);
            break;
        case sf::Event::JoystickConnected:
        case sf::Event::JoystickDisconnected:
            handle_joystick_connect_event(event);
            break;
        default:
            break;  // Ignore other events
    }
}

void InputCapture::handle_close() { window_.close(); }

void InputCapture::handle_key_event(const sf::Event& event) {
    client_.send_message(
        InputMessages::Message(event.type, event.key.code).to_string());
}

void InputCapture::handle_joystick_button_event(const sf::Event& event) {
    client_.send_message(InputMessages::Message(event.type,
                                                event.joystickButton.joystickId,
                                                event.joystickButton.button)
                             .to_string());
}

void InputCapture::handle_joystick_moved(const sf::Event& event) {
    client_.send_message(InputMessages::Message(event.type,
                                                event.joystickButton.joystickId,
                                                event.joystickMove.axis,
                                                event.joystickMove.position)
                             .to_string());
}

void InputCapture::handle_joystick_connect_event(const sf::Event& event) {
    client_.send_message(
        InputMessages::Message(event.type, event.joystickConnect.joystickId)
            .to_string());
}

void InputCapture::stop_client() { io_context_.stop(); }
