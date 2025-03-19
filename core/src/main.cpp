#include <iostream>
#include <string>

std::string getLabelText() { return "Hello from C++!"; }

int main() {
    std::string labelText = getLabelText();
    std::cout << labelText << std::endl;
    return 0;
}
