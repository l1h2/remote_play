#ifndef SFML_TO_WINDOWS_KEY_MAP_HPP
#define SFML_TO_WINDOWS_KEY_MAP_HPP

#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>

#include "windows_virtual_key_codes.hpp"

/**
 * @brief Maps SFML key codes to Windows virtual key codes.
 *
 * This map is used to convert SFML key codes to their corresponding Windows
 * virtual key codes for simulating keyboard input on Windows.
 */
inline const std::unordered_map<sf::Keyboard::Key, int>
    SFML_TO_WINDOWS_KEY_MAP = {
        // Alphabet keys
        {sf::Keyboard::A, VK_A},
        {sf::Keyboard::B, VK_B},
        {sf::Keyboard::C, VK_C},
        {sf::Keyboard::D, VK_D},
        {sf::Keyboard::E, VK_E},
        {sf::Keyboard::F, VK_F},
        {sf::Keyboard::G, VK_G},
        {sf::Keyboard::H, VK_H},
        {sf::Keyboard::I, VK_I},
        {sf::Keyboard::J, VK_J},
        {sf::Keyboard::K, VK_K},
        {sf::Keyboard::L, VK_L},
        {sf::Keyboard::M, VK_M},
        {sf::Keyboard::N, VK_N},
        {sf::Keyboard::O, VK_O},
        {sf::Keyboard::P, VK_P},
        {sf::Keyboard::Q, VK_Q},
        {sf::Keyboard::R, VK_R},
        {sf::Keyboard::S, VK_S},
        {sf::Keyboard::T, VK_T},
        {sf::Keyboard::U, VK_U},
        {sf::Keyboard::V, VK_V},
        {sf::Keyboard::W, VK_W},
        {sf::Keyboard::X, VK_X},
        {sf::Keyboard::Y, VK_Y},
        {sf::Keyboard::Z, VK_Z},

        // Number keys
        {sf::Keyboard::Num0, VK_0},
        {sf::Keyboard::Num1, VK_1},
        {sf::Keyboard::Num2, VK_2},
        {sf::Keyboard::Num3, VK_3},
        {sf::Keyboard::Num4, VK_4},
        {sf::Keyboard::Num5, VK_5},
        {sf::Keyboard::Num6, VK_6},
        {sf::Keyboard::Num7, VK_7},
        {sf::Keyboard::Num8, VK_8},
        {sf::Keyboard::Num9, VK_9},

        // Function keys
        {sf::Keyboard::F1, VK_F1},
        {sf::Keyboard::F2, VK_F2},
        {sf::Keyboard::F3, VK_F3},
        {sf::Keyboard::F4, VK_F4},
        {sf::Keyboard::F5, VK_F5},
        {sf::Keyboard::F6, VK_F6},
        {sf::Keyboard::F7, VK_F7},
        {sf::Keyboard::F8, VK_F8},
        {sf::Keyboard::F9, VK_F9},
        {sf::Keyboard::F10, VK_F10},
        {sf::Keyboard::F11, VK_F11},
        {sf::Keyboard::F12, VK_F12},
        {sf::Keyboard::F13, VK_F13},
        {sf::Keyboard::F14, VK_F14},
        {sf::Keyboard::F15, VK_F15},

        // Arrow keys
        {sf::Keyboard::Left, VK_LEFT},
        {sf::Keyboard::Right, VK_RIGHT},
        {sf::Keyboard::Up, VK_UP},
        {sf::Keyboard::Down, VK_DOWN},

        // Control keys
        {sf::Keyboard::LShift, VK_LSHIFT},
        {sf::Keyboard::RShift, VK_RSHIFT},
        {sf::Keyboard::LControl, VK_LCONTROL},
        {sf::Keyboard::RControl, VK_RCONTROL},
        {sf::Keyboard::LAlt, VK_LMENU},
        {sf::Keyboard::RAlt, VK_RMENU},
        {sf::Keyboard::LSystem, VK_LWIN},
        {sf::Keyboard::RSystem, VK_RWIN},
        {sf::Keyboard::Menu, VK_APPS},
        {sf::Keyboard::Space, VK_SPACE},
        {sf::Keyboard::Enter, VK_RETURN},
        {sf::Keyboard::Backspace, VK_BACK},
        {sf::Keyboard::Tab, VK_TAB},
        {sf::Keyboard::Escape, VK_ESCAPE},

        // Numpad keys
        {sf::Keyboard::Numpad0, VK_NUMPAD0},
        {sf::Keyboard::Numpad1, VK_NUMPAD1},
        {sf::Keyboard::Numpad2, VK_NUMPAD2},
        {sf::Keyboard::Numpad3, VK_NUMPAD3},
        {sf::Keyboard::Numpad4, VK_NUMPAD4},
        {sf::Keyboard::Numpad5, VK_NUMPAD5},
        {sf::Keyboard::Numpad6, VK_NUMPAD6},
        {sf::Keyboard::Numpad7, VK_NUMPAD7},
        {sf::Keyboard::Numpad8, VK_NUMPAD8},
        {sf::Keyboard::Numpad9, VK_NUMPAD9},
        {sf::Keyboard::Add, VK_ADD},
        {sf::Keyboard::Subtract, VK_SUBTRACT},
        {sf::Keyboard::Multiply, VK_MULTIPLY},
        {sf::Keyboard::Divide, VK_DIVIDE},

        // Other keys
        {sf::Keyboard::Insert, VK_INSERT},
        {sf::Keyboard::Delete, VK_DELETE},
        {sf::Keyboard::Home, VK_HOME},
        {sf::Keyboard::End, VK_END},
        {sf::Keyboard::PageUp, VK_PRIOR},
        {sf::Keyboard::PageDown, VK_NEXT},
        {sf::Keyboard::Pause, VK_PAUSE},
        {sf::Keyboard::LBracket, VK_OEM_4},
        {sf::Keyboard::RBracket, VK_OEM_6},
        {sf::Keyboard::Semicolon, VK_OEM_1},
        {sf::Keyboard::Comma, VK_OEM_COMMA},
        {sf::Keyboard::Period, VK_OEM_PERIOD},
        {sf::Keyboard::Apostrophe, VK_OEM_7},
        {sf::Keyboard::Slash, VK_OEM_2},
        {sf::Keyboard::Backslash, VK_OEM_5},
        {sf::Keyboard::Grave, VK_OEM_3},
        {sf::Keyboard::Equal, VK_OEM_PLUS},
        {sf::Keyboard::Hyphen, VK_OEM_MINUS},
};

#endif  // SFML_TO_WINDOWS_KEY_MAP_HPP
