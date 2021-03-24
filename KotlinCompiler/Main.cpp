#include <iostream>

int main(int argc, char** argv) {

    std::cout << "Received command" << std::endl;
    std::cout << argv[0] << " " << argv[1] << " " << argv[2] << std::endl;

    return 0;
}
