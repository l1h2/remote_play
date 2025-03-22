#include <chrono>
#include <iostream>
#include <string>
#include <thread>

std::string getLabelText(bool toggle) {
    return toggle ? "Hello from C++!" : "Goodbye from C++!";
}

int main() {
    bool toggle = true;
    while (true) {
        std::string labelText = getLabelText(toggle);
        std::cout << labelText << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        toggle = !toggle;
    }
    return 0;
}
