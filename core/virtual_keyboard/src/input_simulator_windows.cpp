#include "input_simulator_windows.hpp"

#include <iostream>

#include "sfml_to_windows_key_map.hpp"

InputSimulatorWindows::InputSimulatorWindows() {
    input_ = {};
    input_.type = INPUT_KEYBOARD;
}

void InputSimulatorWindows::press_key(const int sfml_key_code) {
    keydown(sfml_key_code);
    keyup(sfml_key_code);
}

void InputSimulatorWindows::keydown(const int sfml_key_code) {
    key_event(sfml_key_code, 0);
}

void InputSimulatorWindows::keyup(const int sfml_key_code) {
    key_event(sfml_key_code, KEYEVENTF_KEYUP);
}

void InputSimulatorWindows::key_event(const int sfml_key_code,
                                      const DWORD flags) {
    auto virtual_key_code = SFML_TO_WINDOWS_KEY_MAP.find(
        static_cast<sf::Keyboard::Key>(sfml_key_code));

    if (virtual_key_code == SFML_TO_WINDOWS_KEY_MAP.end()) {
        std::cerr << "Key code not found in map: " << sfml_key_code
                  << std::endl;
        return;
    }

    input_.ki.wVk = virtual_key_code->second;
    input_.ki.dwFlags = flags;
    SendInput(1, &input_, INPUT_SIZE);
}
