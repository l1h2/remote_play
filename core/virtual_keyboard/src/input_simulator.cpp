#include "input_simulator.hpp"

#ifdef _WIN32
#include "input_simulator_windows.cpp"
#elif __linux__
#error "Unsupported platform"
#elif __APPLE__
#error "Unsupported platform"
#else
#error "Unsupported platform"
#endif

std::unique_ptr<InputSimulator> InputSimulator::create() {
#ifdef _WIN32
    return std::make_unique<InputSimulatorWindows>();
#elif __linux__
    // Return Linux-specific implementation
#elif __APPLE__
    // Return macOS-specific implementation
#else
    return nullptr;
#endif
}
