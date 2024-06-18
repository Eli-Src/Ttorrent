#ifndef PARSER_H
#define PARSER_H

#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

class Parser {
public:
    struct Option {
        std::string_view description;
        bool requires_argument;
        std::string_view long_identifier;
        std::optional<char> short_identifier;
    };

private:
    std::string_view m_program_name;
    std::unordered_map<std::string_view, Option> m_options;
    std::unordered_map<std::string_view, std::string_view> m_parsed_options;

    void process_long_option(std::string_view option_name, std::string_view option_argument);
    void process_short_option(char option_name, std::string_view option_argument);

public:
    Parser(std::string_view program_name);
    void add_option(std::string_view description,
                    bool requires_argument,
                    std::string_view long_identifier,
                    const std::optional<char>& short_identifier = std::nullopt);
    std::string_view get_program_name() const {return m_program_name;}
    std::string_view get_option_value(std::string_view name) const;
    void parse(int argc, char* argv[]);
    void print_options(std::vector<std::string_view> print_order = {}) const;
};

#endif // PARSER_H
