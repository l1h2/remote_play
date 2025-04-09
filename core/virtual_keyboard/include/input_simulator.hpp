#ifndef INPUT_SIMULATOR_HPP
#define INPUT_SIMULATOR_HPP

#include <cstdint>
#include <memory>

class InputSimulator {
   public:
    virtual ~InputSimulator() = default;

    static std::unique_ptr<InputSimulator> create();

    virtual void press_key(const int sfml_key_code) = 0;
    virtual void keydown(const int sfml_key_code) = 0;
    virtual void keyup(const int sfml_key_code) = 0;
};

#endif  // INPUT_SIMULATOR_HPP
