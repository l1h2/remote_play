#ifndef INPUT_SIMULATOR_WINDOWS_HPP
#define INPUT_SIMULATOR_WINDOWS_HPP

#include <windows.h>

#include "input_simulator.hpp"

constexpr size_t INPUT_SIZE = sizeof(INPUT);

class InputSimulatorWindows : public InputSimulator {
   public:
    InputSimulatorWindows();
    ~InputSimulatorWindows() override = default;

    void press_key(const int sfml_key_code) override;
    void keydown(const int sfml_key_code) override;
    void keyup(const int sfml_key_code) override;

   private:
    void key_event(const int sfml_key_code, const DWORD flags);

    INPUT input_;
};

#endif  // INPUT_SIMULATOR_WINDOWS_HPP
