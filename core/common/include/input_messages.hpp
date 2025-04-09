#ifndef INPUT_MESSAGES_HPP
#define INPUT_MESSAGES_HPP

#include <SFML/Window.hpp>
#include <sstream>
#include <unordered_map>

namespace InputMessages {

enum EventType {
    KEY_PRESSED = 1,
    KEY_RELEASED,
    JOYSTICK_BUTTON_PRESSED,
    JOYSTICK_BUTTON_RELEASED,
    JOYSTICK_MOVED,
    JOYSTICK_CONNECTED,
    JOYSTICK_DISCONNECTED
};

inline const std::unordered_map<sf::Event::EventType, EventType>
    SFML_TO_INPUT_TYPE = {
        {sf::Event::KeyPressed, KEY_PRESSED},
        {sf::Event::KeyReleased, KEY_RELEASED},
        {sf::Event::JoystickButtonPressed, JOYSTICK_BUTTON_PRESSED},
        {sf::Event::JoystickButtonReleased, JOYSTICK_BUTTON_RELEASED},
        {sf::Event::JoystickMoved, JOYSTICK_MOVED},
        {sf::Event::JoystickConnected, JOYSTICK_CONNECTED},
        {sf::Event::JoystickDisconnected, JOYSTICK_DISCONNECTED}};

struct Message {
    EventType type;
    int id;               // Joystick ID or key code
    int button_id;        // Can be axis or button ID
    float axis_position;  // For JOYSTICK_MOVED events

    Message(EventType t, int i, int b = 0, float p = 0)
        : type(t), id(i), button_id(b), axis_position(p) {}

    Message(sf::Event::EventType t, int i, int b = 0, float p = 0)
        : type(SFML_TO_INPUT_TYPE.at(t)),
          id(i),
          button_id(b),
          axis_position(p) {}

    std::string to_string() const {
        std::ostringstream oss;
        oss << static_cast<int>(type) << ":" << id << ":" << button_id << ":"
            << axis_position;
        return oss.str();
    }

    static Message from_string(const std::string& str) {
        std::istringstream iss(str);
        int type_int, id, button_id, axis_position;
        char colon;

        if (!(iss >> type_int >> colon >> id >> colon >> button_id >> colon >>
              axis_position) ||
            colon != ':') {
            throw std::invalid_argument(
                "Invalid input string format for Message::from_string");
        }

        if (type_int < KEY_PRESSED || type_int > JOYSTICK_DISCONNECTED) {
            throw std::out_of_range(
                "Invalid EventType value in Message::from_string");
        }

        EventType type = static_cast<EventType>(type_int);
        return Message(type, id, button_id, axis_position);
    }
};

}  // namespace InputMessages

#endif  // INPUT_MESSAGES_HPP
