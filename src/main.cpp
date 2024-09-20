#include <iostream>
#include <string>
#include <regex>
#include <string_view>

#include "input.h"
#include "parser.h"
#include "bencode.h"
#include "networking.h"

int main(int argc, char* argv[]) {
    constexpr std::string_view node_id = "abcdefghij0123456789";
    constexpr std::string_view address = "router.bittorrent.com";
    constexpr std::string_view port = "6881";

    Parser parser = {"ttorrent"};
    parser.add_option('h', "help", "Display this help message");
    parser.add_option("ml", "Magnet link", Parser::Argument_Types::String_View);

    parser.parse(argc, argv);

    if (parser.get_option_value("help")) {
        parser.print_options({"ml", "", "help"});
        return 0;
    }

    const std::string magnet_link = !parser.get_option_value<std::string_view>("ml") ?
        read_magnet_link() : static_cast<std::string>(*parser.get_option_value<std::string_view>("ml"));

    // check if magnet link has right syntax
    const std::regex magnet_link_regex("^magnet:\\?xt=urn:btih:[a-zA-Z0-9]+$");
    if (!std::regex_match(magnet_link, magnet_link_regex)) {
        std::cerr << "Magnet link has wrong format.\n";
        return 1;
    }

    std::string magnet_id = magnet_link.substr(20);
    Bencode ben(Bencode::Map{
        {"t", "ohmygawd"},
        {"y", "q"},
        {"q", "get_peers"},
        {"a", Bencode::Nested_Map{
            {"id", node_id},
            {"info_hash", magnet_id},
        }},
    });

    Client client(address, port);
    std::string msg = client.send_and_recieve(ben.encode());
    // std::cout << msg << '\n';
    // std::cout << size(msg) << '\n';
    // Bencode::Map decoded_msg = Bencode::decode(msg);
    // for (const auto& pair : decoded_msg) {
    //     std::cout << pair.first << '\n';
    // };
    // std::string ip;
    // int port;
    // std::string compact_ip = static_cast<std::string>(std::get<std::string_view>(map["ip"]));
    // unpack_compact_ip(compact_ip, ip, port);

    return 0;
}
