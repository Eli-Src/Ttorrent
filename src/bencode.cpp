#include <cassert>
#include <charconv>
#include <queue>
#include <sstream>

#include "bencode.h"

Bencode::Bencode(Map map) :
    m_map(map)
{}

std::optional<int> Bencode::to_int(std::string_view input) {
    int out;
    const char *end = input.begin() + input.size();
    const auto result = std::from_chars(input.begin(), end, out);
    if(result.ec == std::errc::invalid_argument ||
        result.ec == std::errc::result_out_of_range ||
        result.ptr != end) {
        return std::nullopt;
    }
    return out;
}

int Bencode::get_value_length(std::string_view input_string, size_t num_start_pos, size_t& difference) {
    size_t num_end_pos = input_string.find_first_of(':', num_start_pos);
    difference = num_end_pos - 1 - num_start_pos;

    int length;
    if (!difference)
        length = input_string[num_start_pos] - '0';
    else
        length = *to_int(input_string.substr(num_start_pos, difference+1)); 

    return length;
}

std::string Bencode::string() const {
    std::stringstream stream;
    stream << "{";
    for (auto it = m_map.begin(); it != m_map.end(); ++it) {
        stream << '\"' << it->first << '\"' << ':';
        if (std::holds_alternative<Nested_Map>(it->second)) {
            stream << '{';
            const auto& map = std::get<Nested_Map>(it->second);
            for (auto it_2 = map.begin(); it_2 != map.end(); ++it_2) {
                // stream << '\"' << it_2->first << '\"' << ':' << '\"' << it_2->second << "\"";
                if (std::next(it_2) != map.end()) stream << ',';
            }
            stream << '}';
        } else {
            const auto string_view = std::get<std::string_view>(it->second);
            stream << '\"' << string_view << '\"';
        }
        if (std::next(it) != m_map.end()) stream << ',';
    }
    stream << '}';

    return stream.str();
}

// TODO: Missing lists and integers
std::string Bencode::encode() const {
    std::stringstream stream;
    std::string string = this->string();
    for (size_t i = 0; i < string.length(); ++i) {
        switch (string[i]) {
            case '{':
                stream << 'd';
                break;
            case '"': {
                size_t pos = string.find_first_of('"', i+1);
                size_t difference  = pos - i - 1;
                stream << difference << ':' << string.substr(i+1, difference);
                i = pos;
                break;
            }
            case '}':
                stream << 'e';
                break;
            default:
                break;
        }
    }

    return stream.str();
}

Bencode::Map Bencode::decode(std::string_view string) {
    Bencode::Map map;
    Bencode::Nested_Map nested_map;
    Bencode::Nested_List nested_list;
    std::string_view nested_map_key = "";
    std::string_view nested_list_key = "";
    bool is_first_nested_map_pair = false;
    bool is_first_nested_list_item = false;
    std::queue<char> nest_queue;

    for (size_t i = 0; i < size(string) - 1; ++i) {
        char c = string[i];
        if (c == 'e') {
            assert(!nest_queue.empty());
            switch (nest_queue.front()) {
                case 'd': {
                    is_first_nested_map_pair = false;
                    map[nested_map_key] = nested_map;
                    nested_map = {};
                    break;
                }
                case 'l': {
                    is_first_nested_list_item = false;
                    if (nest_queue.size() >= 2) nested_map[nested_list_key] = nested_list;
                    else map[nested_list_key] = nested_list;
                    nested_list = {};
                    break;
                }
            }
            nest_queue.pop();
        } else if (c >= '1' && c <= '9') {
            size_t difference = 0;

            int key_length = Bencode::get_value_length(string, i, difference);
            std::string_view key = string.substr(i + 2 + difference, key_length);

            size_t value_length_pos = i + 2 + key_length;
            switch (string[value_length_pos]) {
                case 'l': {
                    nest_queue.push('l');
                    is_first_nested_list_item = true;
                    ++value_length_pos;
                    break;
                }
                case 'd': {
                    nest_queue.push('d');
                    is_first_nested_map_pair = true;
                    ++value_length_pos;
                    break;
                }
            }

            if (nest_queue.empty()) {
                int value_length = Bencode::get_value_length(string, value_length_pos, difference);
                std::string_view value = string.substr(value_length_pos + 2 + difference, value_length);
                map[key] = value;
                i = value_length_pos + 2 + value_length + difference - 1;
            } else {
                switch (nest_queue.front()) {
                    case 'l': {
                        if (is_first_nested_list_item) {
                            int value_length = Bencode::get_value_length(string, value_length_pos, difference);
                            std::string_view value = string.substr(value_length_pos + 2 + difference, value_length);
                            nested_list.emplace_back(value);
                            nested_list_key = key;
                            is_first_nested_list_item = false;
                            i = value_length_pos + 2 + value_length + difference - 1;
                        } else {
                            nested_list.emplace_back(key);
                            i = value_length_pos - 1;
                        }
                        break;
                    }
                    case 'd': {
                        int value_length = Bencode::get_value_length(string, value_length_pos, difference);
                        std::string_view value = string.substr(value_length_pos + 2 + difference, value_length);
                        if (is_first_nested_map_pair) {
                            size_t nested_value_length_pos = value_length_pos + 2 + value_length;
                            int nested_value_length = Bencode::get_value_length(string, nested_value_length_pos, difference);
                            std::string_view nested_value = string.substr(nested_value_length_pos + 2 + nested_value_length - 1, nested_value_length);

                            nested_map[value] = nested_value;
                            nested_map_key = key;
                            is_first_nested_map_pair = false;
                            i = nested_value_length_pos + 2 + nested_value_length + difference - 1;
                        } else {
                            nested_map[key] = value;
                            i = value_length_pos + 2 + value_length + difference - 1;
                        }
                        break;
                    }   
                }
            }
        }
    }
    
    if (!nested_map.empty() && !nested_map_key.empty()) {
        map[nested_map_key] = nested_map;
    }

    return map;
}
