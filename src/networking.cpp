#include <string_view>
#include <vector>
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>

#include "networking.h"

Client::Client(std::string_view address, std::string_view port) :
    m_address(address),
    m_port(port)
{}

std::string Client::send_and_recieve(std::string_view message) {
    asio::error_code err;

    asio::io_service io_service;
    asio::ip::udp::resolver resolver(io_service);
    asio::ip::udp::resolver::query query(asio::ip::udp::v4(), m_address, m_port);
    auto iter = resolver.resolve(query);
    asio::ip::udp::endpoint endpoint = iter->endpoint();
    asio::ip::udp::socket socket(io_service, asio::ip::udp::v4());
    // std::cout << "Connected on: " << endpoint.address() << ':' << endpoint.port() << '\n';

    socket.send_to(asio::buffer(message.data(), message.size()), endpoint);
    // std::cout << "Sent: " << msg << '\n';

    std::vector<char> recv_buf(10 * 1024);
    socket.receive_from(asio::buffer(recv_buf, recv_buf.size()), endpoint);

    return recv_buf.data();
}

// void unpack_compact_ip(const std::string compact_ip, std::string& ip, int& port) {
//     if (compact_ip.size() != 6) {
//         throw std::invalid_argument("Invalid compact info size");
//     }
//
//     // Extract IP address
//     int ip_binary = 0;
//     for (size_t i = 0; i < 4; ++i) {
//         ip_binary |= (static_cast<unsigned char>(compact_ip[i]) << (24 - 8 * i));
//     }
//
//     in_addr ip_addr;
//     ip_addr.s_addr = ip_binary;
//     ip = inet_ntoa(ip_addr);
//     
//     // Extract port number
//     int port_net = (static_cast<unsigned char>(compact_ip[4]) << 8) |
//                         static_cast<unsigned char>(compact_ip[5]);
//     port = ntohs(port_net);
// }

// std::string ip;
// int port;
// std::string compact_ip = static_cast<std::string>(std::get<std::string_view>(map["ip"]));
// unpack_compact_ip(compact_ip, ip, port);
