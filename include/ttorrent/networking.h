#ifndef NETWORKING_H
#define NETWORKING_H

#include <string>

class Client {
private:
    std::string m_address;
    std::string m_port;

public:
    Client(std::string_view address, std::string_view port);
    std::string send_and_recieve(std::string_view message);
};

#endif // NETWORKING_H
