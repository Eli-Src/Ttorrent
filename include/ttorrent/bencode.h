#ifndef BENCODE_H
#define BENCODE_H

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

class Bencode {
public:
    using Nested_List = std::vector<std::string_view>;
    using Nested_Map = std::unordered_map<std::string_view, std::variant<std::string_view, Nested_List>>;
    using Map_Value = std::variant<std::string_view, Nested_Map, Nested_List>;
    using Map = std::unordered_map<std::string_view, Map_Value>;

private:
    Map m_map;

    static std::optional<int> to_int(std::string_view input);
    static int get_value_length(std::string_view input_string, size_t num_start_pos, size_t& difference);

public:
    Bencode(Bencode::Map map);
    std::string string() const;
    std::string encode() const;
    static Bencode::Map decode(std::string_view string);
    friend std::ostream& operator <<(std::ostream& os, const Bencode obj) {
        os << obj.string();
        return os;
    }
};

#endif // BENCODE_H
