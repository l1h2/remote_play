#ifndef INPUT_SIMULATOR_HPP
#define INPUT_SIMULATOR_HPP

#include <cstdint>
#include <memory>

/**
 * @class InputSimulator
 * @brief Interface for simulating keyboard inputs. To be implemented by
 * platform-specific classes.
 */
class InputSimulator {
   public:
    /**
     * @brief Destroy the InputSimulator object
     */
    virtual ~InputSimulator() = default;

    /**
     * @brief Create a new InputSimulator object.
     *
     * @return std::unique_ptr<InputSimulator> Pointer to the created object.
     */
    static std::unique_ptr<InputSimulator> create();

    /**
     * @brief Simulate a key press event. Keydown and keyup.
     *
     * @param sfml_key_code SFML key code to simulate.
     */
    virtual void press_key(const int sfml_key_code) = 0;

    /**
     * @brief Simulate a keydown event.
     *
     * @param sfml_key_code SFML key code to simulate.
     */
    virtual void keydown(const int sfml_key_code) = 0;

    /**
     * @brief Simulate a keyup event.
     *
     * @param sfml_key_code SFML key code to simulate.
     */
    virtual void keyup(const int sfml_key_code) = 0;
};

#endif  // INPUT_SIMULATOR_HPP
