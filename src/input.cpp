#include <iostream>

void clear_line() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool clear_failed_extraction() {
    if (!std::cin) {
        // If the stream was closed
        if (std::cin.eof()) {
            exit(0);
        }

        // Put us back in 'normal' operation mode
        std::cin.clear(); 
        clear_line();

        return true;
    }

    return false;
}

std::string read_magnet_link() {
    while (true) {
        std::cout << "Please enter magnet_link: ";
        std::string magnet_link;
        std::cin >> magnet_link;

        if (clear_failed_extraction()) {
            std::cout << "Error in handling input. Please try again.\n";
            continue;
        }

        clear_line();
        return magnet_link;
    }
}
