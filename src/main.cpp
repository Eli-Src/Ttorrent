#include <iostream>
#include <string>
#include <regex>

#include "ttorrent/parser.h"

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

int main(int argc, char* argv[]) {
    Parser parser = {"ttorrent"};
    parser.add_option("Display this help message", false, "help", 'h');
    parser.add_option("Magnet link", true, "ml");

    parser.parse(argc, argv);

    if (!parser.get_option_value("help").empty()) {
        parser.print_options();
        return 0;
    }

    std::string magnet_link = static_cast<std::string>(parser.get_option_value("ml"));
    if (magnet_link.empty()) {
        magnet_link = read_magnet_link();
    }

    // check if magnet link has right syntax
    const std::regex magnet_link_regex("^magnet:\\?xt=urn:btih:[a-zA-Z0-9]+$");
    if (!std::regex_match(magnet_link, magnet_link_regex)) {
        std::cerr << "Magnet link has wrong format.\n";
        return 1;
    }

    std::cout << parser.get_program_name() << ": " << magnet_link << '\n';

    return 0;
}
